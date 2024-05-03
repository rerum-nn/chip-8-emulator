#include "memory.h"

void Memory::LoadFont() {
    std::copy(kFont.begin(), kFont.end(), memory_.begin());
}
uint16_t Memory::GetFontLocation(uint8_t value) const {
    return kFontHeight * (value & 0x0F);
}
uint8_t Memory::operator[](uint16_t address) const {
    return memory_[address];
}
uint8_t& Memory::operator[](uint16_t address) {
    return memory_[address];
}
Opcode Memory::ReadOpcode(uint16_t address) const {
    Opcode result;
    result.opcode = (memory_[address] << 8) | memory_[address + 1];
    return result;
}
const uint8_t* Memory::GetVideoMemory() const {
    return &memory_.data()[kVideoMemoryAddress];
}
uint8_t* Memory::GetVideoMemory() {
    return &memory_.data()[kVideoMemoryAddress];
}

Memory::Memory() {
    std::fill(memory_.begin(), memory_.end(), 0);
    LoadFont();
}
uint8_t* Memory::GetInputMemory() {
    return &memory_.data()[kInputAddress];
}
