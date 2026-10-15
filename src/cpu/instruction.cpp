#include "psx/instruction.hpp"

std::uint32_t Instruction::func() {
    return this->opcode >> 26;
}
std::uint32_t Instruction::imm() {
    return this->opcode & 0xFFFF;
}
std::uint32_t Instruction::rt() {
    return (this->opcode >> 16) & 0x1F;
}
std::uint32_t Instruction::rs() {
    return (this->opcode >> 21) & 0x1F;
}
std::uint32_t Instruction::rd() {}