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

    // Set n bytes start from address from buf
    void Set(uint16_t address, const uint8_t* buf, size_t n);
    // Load n bytes start from address to buf
    void Load(uint16_t address, uint8_t* buf, size_t n) const;

private:
    std::array<uint8_t , kMemorySize> memory_;
};
