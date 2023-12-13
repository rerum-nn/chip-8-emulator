#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class Memory {
private:
    static const size_t kMaxAddress = 0xFFF;
public:
    static const size_t kMemorySize = 4096;

    Memory();

    void StoreDump(const std::array<uint8_t , kMemorySize>& dumped_memory);
    std::array<uint8_t , kMemorySize> SaveDump() const;

    void Set1(uint16_t address, uint8_t byte);
    void Set2(uint16_t address, uint16_t byte);
    void Set4(uint16_t address, uint32_t byte);
    void Set8(uint16_t address, uint64_t byte);

    uint8_t Load1(uint16_t address) const;
    uint16_t Load2(uint16_t address) const;
    uint32_t Load4(uint16_t address) const;
    uint64_t Load8(uint16_t address) const;

private:
    std::array<uint8_t , kMemorySize> memory_;
};
