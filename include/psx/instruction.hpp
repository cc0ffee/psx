#pragma once
#include <cstdint>

class Instruction {
public:
    explicit Instruction(const uint32_t& opcode) : opcode(opcode) {}
    std::uint32_t op();
    std::uint32_t funct();
    std::uint32_t imm();
    std::uint32_t imm_se();
    std::uint32_t rt();
    std::uint32_t rs();
    std::uint32_t rd();
    std::uint32_t shamt();
    std::uint32_t target();

    std::uint32_t opcode{};
};