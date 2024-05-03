#pragma once

#include <array>
#include <unordered_map>

#include "cartridge.h"
#include "display.h"
#include "memory.h"
#include "opcode.h"

class ChipKernel {
private:
    static constexpr size_t kGRP = 16;  // General Purpose Registers amount

public:
    ChipKernel(size_t hertz = 2000, const Memory& memory = Memory());

    void StoreMemory(const Memory& memory);
    void StoreMemory(Memory&& memory);
    void StoreMemory(std::array<uint8_t, Memory::kMemorySize>);

    const Memory& GetMemory() const;

    void LoadProgram(const Cartridge& cartridge);

    void Run();

    std::unordered_map<char, uint16_t> DumpRegisters() const;
    void StoreRegisters(const std::unordered_map<char, uint16_t>& registers);

    // TODO: Errors dispatcher (may be or may be not)

private:
    void PushStack(uint16_t value);
    uint16_t PopStack();

    void UpdatePcReg(uint16_t value);
    void NextInstruction();

    Opcode ReadOpcode();
    void ExecuteOpcode(Opcode opcode);

    void FlowControl(Opcode opcode);
    void MathAndLogic(Opcode opcode);
    void SpecialRegisters(Opcode opcode);

    static uint8_t GetRandomByte();

    static constexpr uint32_t kStackSize = 16;
    static constexpr uint16_t kExecutionStart = 0x200;
    static constexpr uint64_t kRandomSeed = 1337;

    static constexpr size_t kDisplayWidth = 64;
    static constexpr size_t kDisplayHeight = 32;

    Display display_;
    Memory memory_;

    std::array<uint8_t, kGRP> grp_regs_;
    uint16_t i_reg_;
    uint16_t pc_reg_;
    uint16_t sp_reg_;

    uint8_t delay_timer_;
    uint8_t sound_timer_;

    std::array<uint16_t, kStackSize> stack_reg_;

    bool is_ready_to_display_;

    size_t cpu_hertz_;

    std::string cartridge_name_;
};
