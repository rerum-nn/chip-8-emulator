#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "opcode.h"

class Memory {
private:
    static constexpr size_t kMaxAddress = 0xFFF;
public:
    static constexpr size_t kMemorySize = 4096;
    static constexpr size_t kVideoMemorySize = 256;

    Memory();

    uint16_t GetFontLocation(uint8_t value) const;

    uint8_t operator[](uint16_t address) const;
    uint8_t& operator[](uint16_t address);

    Opcode ReadOpcode(uint16_t address) const;

    const uint8_t* GetVideoMemory() const;
    uint8_t* GetVideoMemory();

    uint8_t* GetInputMemory();

    void StoreDump(const std::array<uint8_t , kMemorySize>& dumped_memory);
    std::array<uint8_t , kMemorySize> SaveDump() const;

private:
    static constexpr uint16_t kFontHeight = 5;
    static constexpr uint16_t kFontSize = 16;
    static constexpr uint16_t kFontBytesSize = kFontHeight * kFontSize;
    static constexpr uint16_t kFontAddress = 0x0;

    static constexpr uint16_t kVideoMemoryAddress = 0xF00;

    static constexpr uint16_t kInputAddress = kFontAddress + kFontBytesSize;

    static constexpr std::array<uint8_t, kFontBytesSize> kFont = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    void LoadFont();

    std::array<uint8_t, kMemorySize> memory_;
};

