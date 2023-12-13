#pragma once

#include <array>
#include <unordered_map>
#include "memory.h"
#include "opcode.h"

class ChipKernel {
private:
    static const size_t kGRP = 16; // General Purpose Registers amount

public:
    ChipKernel();

    void StoreMemory(const Memory& memory);
    void StoreMemory(Memory&& memory);
    void StoreMemory(std::array<uint8_t, Memory::kMemorySize>);

    const Memory& GetMemory() const;

    void LoadProgram(uint8_t* programm, size_t n);

    void Run();

    std::unordered_map<char, uint16_t> DumpRegisters() const;
    void StoreRegisters(const std::unordered_map<char, uint16_t>& registers);

    // TODO: Errors dispatcher (may be or may be not)

private:
    static const uint32_t kStackSize = 16;

    Memory memory_;
    std::array<uint16_t, kGRP> grp_regs_;
    uint16_t i_reg_;
    uint16_t pc_reg_;

    uint8_t wait_timer_;
    uint8_t sound_timer_;

    uint16_t stack_reg_[kStackSize];

    void Execute(); // Program processing

};
