#include "../include/psx/cpu.hpp"
#include <iostream>

void CPU::run() {
    current_pc = pc;
    Instruction instruction(bus.read<std::uint32_t>(pc));

    pc = next_pc;
    next_pc += 4;

    decodeExec(instruction);
}

void CPU::decodeExec(Instruction& instruction) {
    switch(instruction.func()) {
        case 0x0D:
            this->OP_ORI(instruction);
            break;
        case 0x0F:
            this->OP_LUI(instruction);
            break;
        default:
            std::cerr << "[CPU] Unhandled Instruction: " << std::hex << instruction.opcode << "(0x" << instruction.func() << ")" << std::endl;
            throw std::exception();
    }
}

std::uint32_t CPU::getReg(std::uint32_t reg) {

}
void CPU::setReg(std::uint32_t reg, const std::uint32_t &value) {
    this->gRegs[reg] = value;
}

void CPU::OP_ORI(Instruction& instruction) {
    std::uint32_t imm = instruction.imm();
    std::uint32_t rs = instruction.rs();
    std::uint32_t rt = instruction.rt();

    std::uint32_t value = this->gRegs[rs] | imm;

    this->setReg(rt, value);
}

void CPU::OP_LUI(Instruction& instruction) {
    std::uint32_t imm = instruction.imm();
    std::uint32_t rt = instruction.rt();
    std::uint32_t value = imm << 16u;

    this->setReg(rt, value);
}