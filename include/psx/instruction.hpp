#pragma once
#include <cstdint>

class Instruction {
public:
    explicit Instruction(const uint32_t& opcode) : opcode(opcode) {}
    std::uint32_t func();
    std::uint32_t imm();
    std::uint32_t rt();
    std::uint32_t rs();
    std::uint32_t rd();

    std::uint32_t opcode{};
};