#include "psx/instruction.hpp"

std::uint32_t Instruction::op() {
    return this->opcode >> 26;
}

std::uint32_t Instruction::funct() {
    return this->opcode & 0x3F;
}

std::uint32_t Instruction::imm() {
    return this->opcode & 0xFFFF;
}

std::uint32_t Instruction::imm_se() {
    return static_cast<std::uint32_t>(static_cast<std::int16_t>(this->opcode & 0xFFFF));
}

std::uint32_t Instruction::rt() {
    return (this->opcode >> 16) & 0x1F;
}
std::uint32_t Instruction::rs() {
    return (this->opcode >> 21) & 0x1F;
}
std::uint32_t Instruction::rd() {
    return (this->opcode >> 11) & 0x1F;
}

std::uint32_t Instruction::shamt() {
    return (this->opcode >> 6) & 0x1F;
}

std::uint32_t Instruction::target() {
    return this->opcode & 0x3FFFFFF;
}