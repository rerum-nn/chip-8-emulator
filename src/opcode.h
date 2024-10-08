#pragma once

#include <cstdint>

constexpr static size_t kOpcodeSize = 2;

union Opcode {
    uint16_t opcode;

    // General view
    struct {
        uint16_t b4 : 4;
        uint16_t b3 : 4;
        uint16_t b2 : 4;
        uint16_t b1 : 4;
    } pos;

    // Operator which require address
    struct {
        uint16_t address : 12;
        uint16_t type : 4;
    } address;

    // Operations which require constants
    struct {
        uint16_t value : 8;
        uint16_t reg : 4;
        uint16_t type : 4;
    } constant;

    // Math and logic operations
    struct {
        uint16_t op : 4;
        uint16_t y : 4;
        uint16_t x : 4;
        uint16_t type : 4;
    } reg;
};
