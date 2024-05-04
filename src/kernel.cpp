#include "kernel.h"

#include <cstdint>
#include <random>

#include "speakers.h"

ChipKernel::ChipKernel(size_t hertz, const Memory& memory)
    : memory_(memory),
      i_reg_(0),
      pc_reg_(0),
      sp_reg_(0),
      delay_timer_(0),
      sound_timer_(0),
      is_ready_to_display_(false),
      cpu_hertz_(hertz) {
    std::fill(grp_regs_.begin(), grp_regs_.end(), 0);
    std::fill(stack_reg_.begin(), stack_reg_.end(), 0);
}

void ChipKernel::PushStack(uint16_t value) {
    stack_reg_[sp_reg_++] = value;
}

uint16_t ChipKernel::PopStack() {
    return stack_reg_[--sp_reg_];
}

void ChipKernel::UpdatePcReg(uint16_t value) {
    pc_reg_ = value;
}
void ChipKernel::NextInstruction() {
    pc_reg_ += 2;
}
void ChipKernel::Run(Display& display, Speakers& speakers) {
    display.ConnectVideoMemory(memory_.GetVideoMemory());
    display.SetWindowName(cartridge_name_);

    pc_reg_ = kExecutionStart;

    Opcode current_op;

    sf::Time cpu_period = sf::seconds(1. / cpu_hertz_);
    sf::Time timer_period = sf::seconds(1. / 60.);
    sf::Time fps_period = sf::seconds(1. / 120.);

    sf::Clock kernel_clock;
    sf::Clock timer_clock;
//    sf::Clock fps_clock;
    sf::Time last_update = sf::Time::Zero;
    sf::Time last_timer_update = sf::Time::Zero;
//    sf::Time last_fps_update = sf::Time::Zero;

    while (display.IsOpen()) {
        last_update += kernel_clock.restart();
        last_timer_update += timer_clock.restart();
//        last_fps_update += fps_clock.restart();
        while (display.IsOpen() && last_update > cpu_period) {
            last_update -= cpu_period;
            current_op = ReadOpcode();
            display.CheckKeyboard(memory_.GetInputMemory());
            ExecuteOpcode(current_op);
            if (is_ready_to_display_) {
                is_ready_to_display_ = false;
                display.Update();
            }
        }
//        while (display.IsOpen() && last_fps_update > fps_period) {
//            last_fps_update -= fps_period;
//            display.Update();
//        }
        while (display.IsOpen() && last_timer_update > timer_period) {
            last_timer_update -= timer_period;
            if (delay_timer_ > 0) {
                --delay_timer_;
            }
            if (sound_timer_ > 0) {
                --sound_timer_;
                if (!speakers.AreActive()) {
                    speakers.Play();
                }
                if (sound_timer_ == 0 && speakers.AreActive()) {
                    speakers.Stop();
                }
            }
        }
    }
}

void ChipKernel::LoadProgram(const Cartridge& cartridge) {
    const std::vector<uint8_t> data = cartridge.GetData();
    for (size_t pos = 0; pos < data.size(); ++pos) {
        memory_[kExecutionStart + pos] = data[pos];
    }
    cartridge_name_ = cartridge.GetName();
}

Opcode ChipKernel::ReadOpcode() {
    Opcode result = memory_.ReadOpcode(pc_reg_);
    return result;
}
uint8_t ChipKernel::GetRandomByte() {
    static std::uniform_int_distribution<uint8_t> uid(0, UINT8_MAX);
    static std::mt19937 generator(kRandomSeed);
    return uid(generator);
}
void ChipKernel::ExecuteOpcode(Opcode opcode) {
    if (0x0 <= opcode.pos.b1 && opcode.pos.b1 <= 0x5 || opcode.pos.b1 == 0x9 ||
        opcode.pos.b1 == 0xB) {
        FlowControl(opcode);
    } else if (0x6 <= opcode.pos.b1 && opcode.pos.b1 <= 0x8 || opcode.pos.b1 == 0xC) {
        MathAndLogic(opcode);
    } else {
        SpecialRegisters(opcode);
    }
}
void ChipKernel::FlowControl(Opcode opcode) {
    switch (opcode.pos.b1) {
        case 0x0:
            switch (opcode.opcode) {
                // CLS
                case 0x00E0: {
                    uint8_t* video_memory = memory_.GetVideoMemory();
                    for (size_t i = 0; i < Memory::kVideoMemorySize; ++i) {
                        video_memory[i] = 0;
                    }
                    is_ready_to_display_ = true;
                    break;
                }
                // RET
                case 0x00EE:
                    UpdatePcReg(PopStack());
                    break;
                // SYS
                default:
                    // TODO: Log that SYS is legacy
                    //                    UpdatePcReg(opcode.address.address);
                    break;
            }
            NextInstruction();
            break;
        // JP
        case 0x1:
            UpdatePcReg(opcode.address.address);
            break;
        // CALL
        case 0x2:
            PushStack(pc_reg_);
            UpdatePcReg(opcode.address.address);
            break;
        // SE
        case 0x3:
            if (grp_regs_[opcode.constant.reg] == opcode.constant.value) {
                NextInstruction();
            }
            NextInstruction();
            break;
        // SNE
        case 0x4:
            if (grp_regs_[opcode.constant.reg] != opcode.constant.value) {
                NextInstruction();
            }
            NextInstruction();
            break;
        // SE
        case 0x5:
            if (grp_regs_[opcode.reg.x] == grp_regs_[opcode.reg.y]) {
                NextInstruction();
            }
            NextInstruction();
            break;
        // SNE
        case 0x9:
            if (grp_regs_[opcode.reg.x] != grp_regs_[opcode.reg.y]) {
                NextInstruction();
            }
            NextInstruction();
            break;
        // JP
        case 0xB:
            UpdatePcReg(grp_regs_[0] + opcode.address.address);
            break;
    }
}
void ChipKernel::MathAndLogic(Opcode opcode) {
    uint8_t& reg_x = grp_regs_[opcode.reg.x];
    uint8_t& reg_y = grp_regs_[opcode.reg.y];
    uint8_t& reg_f = grp_regs_[0xF];

    switch (opcode.reg.type) {
        case 0x6:
            reg_x = opcode.constant.value;
            break;
        case 0x7:
            reg_x += opcode.constant.value;
            break;
        case 0x8:
            switch (opcode.reg.op) {
                case 0x0:
                    reg_x = reg_y;
                    break;
                case 0x1:
                    reg_x |= reg_y;
                    break;
                case 0x2:
                    reg_x &= reg_y;
                    break;
                case 0x3:
                    reg_x ^= reg_y;
                    break;
                case 0x4: {
                    uint8_t tmp = reg_x + reg_y;
                    reg_f = !(tmp >= reg_x && tmp >= reg_y) ? 1 : 0;
                    reg_x += reg_y;
                    break;
                }
                case 0x5:
                    reg_f = reg_x > reg_y ? 1 : 0;
                    reg_x -= reg_y;
                    break;
                case 0x6:
                    reg_f = reg_x & 0x01;
                    reg_x >>= 1;
                    break;
                case 0x7:
                    reg_f = reg_y > reg_x ? 1 : 0;
                    reg_x = reg_y - reg_x;
                    break;
                case 0xE:
                    reg_f = reg_x >> 7;
                    reg_x <<= 1;
                    break;
            }
            break;
        case 0xC:
            reg_x = GetRandomByte() % opcode.constant.value;
            break;
    }
    NextInstruction();
}
void ChipKernel::SpecialRegisters(Opcode opcode) {
    switch (opcode.pos.b1) {
        case 0xA:
            i_reg_ = opcode.address.address;
            break;
        case 0xD: {
            uint16_t y_coord = grp_regs_[opcode.reg.y] % kDisplayHeight;
            uint16_t x_coord = grp_regs_[opcode.reg.x] % kDisplayWidth;
            uint8_t* video_memory = memory_.GetVideoMemory();

            grp_regs_[0xF] = 0;
            for (uint8_t i = 0; i < opcode.reg.op; ++i) {
                uint8_t byte = memory_[i_reg_ + i];
                uint16_t current_y = (y_coord + i) % kDisplayHeight;
                for (uint8_t j = 0; j < 8; ++j) {
                    uint16_t current_x = (x_coord + j) % kDisplayWidth;
                    uint8_t color = (byte >> (7 - j)) & 0x01;
                    uint8_t& video_memory_byte =
                        video_memory[(current_y * kDisplayWidth + current_x) / 8];
                    uint8_t display_color = (video_memory_byte >> (7 - current_x % 8)) & 0x01;
                    if (color != 0) {
                        if (color & display_color) {
                            grp_regs_[0xF] = 1;
                        }
                        if (display_color) {
                            video_memory_byte &= (~(0x1 << (7 - current_x % 8)));
                        } else {
                            video_memory_byte |= (0x1 << (7 - current_x % 8));
                        }
                    }
                }
            }
            is_ready_to_display_ = true;
            break;
        }
        case 0xE:
            switch (opcode.constant.value) {
                case 0x9E:
                    if (memory_.GetInputMemory()[grp_regs_[opcode.constant.reg]]) {
                        NextInstruction();
                    }
                    break;
                case 0xA1:
                    if (!memory_.GetInputMemory()[grp_regs_[opcode.constant.reg]]) {
                        NextInstruction();
                    }
                    break;
            }
            break;
        case 0xF:
            switch (opcode.constant.value) {
                case 0x07:
                    grp_regs_[opcode.constant.reg] = delay_timer_;
                    break;
                case 0x0A: {
                    bool is_pressed = false;
                    for (size_t i = 0; i < 16; ++i) {
                        if (memory_.GetInputMemory()[i]) {
                            grp_regs_[opcode.constant.reg] = i;
                            is_pressed = true;
                            break;
                        }
                    }
                    if (!is_pressed) {
                        return;
                    }
                    break;
                }
                case 0x15:
                    delay_timer_ = grp_regs_[opcode.constant.reg];
                    break;
                case 0x18:
                    sound_timer_ = grp_regs_[opcode.constant.reg];
                    break;
                case 0x1E:
                    i_reg_ += grp_regs_[opcode.constant.reg];
                    break;
                case 0x29:
                    i_reg_ = memory_.GetFontLocation(grp_regs_[opcode.constant.reg]);
                    break;
                case 0x33: {
                    uint8_t reg_x = grp_regs_[opcode.reg.x];
                    uint8_t h = reg_x / 100;
                    uint8_t t = (reg_x - h * 100) / 10;
                    uint8_t o = reg_x - h * 100 - t * 10;

                    memory_[i_reg_] = h;
                    memory_[i_reg_ + 1] = t;
                    memory_[i_reg_ + 2] = o;
                    break;
                }
                case 0x55:
                    for (uint16_t reg = 0; reg <= opcode.reg.x; ++reg) {
                        memory_[i_reg_ + reg] = grp_regs_[reg];
                    }
                    //                    i_reg_ += opcode.reg.x + 1;
                    break;
                case 0x65:
                    for (uint16_t reg = 0; reg <= opcode.reg.x; ++reg) {
                        grp_regs_[reg] = memory_[i_reg_ + reg];
                    }
                    //                    i_reg_ += opcode.reg.x + 1;
                    break;
            }
            break;
    }
    NextInstruction();
}
