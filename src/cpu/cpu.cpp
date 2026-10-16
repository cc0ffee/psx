#include "../include/psx/cpu.hpp"
#include <iostream>

void CPU::run() {
    Instruction instruction(bus.read<std::uint32_t>(pc));

    std::uint32_t next_pc = delayed_branch.value_or(this->pc + 4);
    delayed_branch.reset();
    pc = next_pc;

    decodeExec(instruction);
}

void CPU::decodeExec(Instruction& instruction) {
    switch(instruction.op()) {
        case 0x00: {
            switch(instruction.funct()) {
                case 0x00:
                    this->OP_SLL(instruction);
                    break;
                case 0x25:
                    this->OP_OR(instruction);
                    break;
                default:
                    std::cerr << "[CPU] Unhandled 0x00 Instruction: 0x" << std::hex << instruction.funct() << std::endl;
                    throw std::exception();
            }
            break;
        }
        case 0x02:
            this->OP_J(instruction);
            break;
        case 0x09:
            this->OP_ADDIU(instruction);
            break;
        case 0x0D:
            this->OP_ORI(instruction);
            break;
        case 0x0F:
            this->OP_LUI(instruction);
            break;
        case 0x10:
            this->OP_COP0(instruction);
        case 0x2B:
            this->OP_SW(instruction);
            break;
        default:
            std::cerr << "[CPU] Unhandled Instruction: " << std::hex << instruction.opcode << "(0x" << instruction.op() << ")" << std::endl;
            throw std::exception();
    }
}

std::uint32_t CPU::getReg(std::uint32_t reg) {
    return this->gRegs[reg];
}
void CPU::setReg(std::uint32_t reg, const std::uint32_t &value) {
    this->gRegs[reg] = value;
    this->gRegs[0] = 0;
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

void CPU::OP_SW(Instruction& instruction) {
    if ((this->sr & 0x10000) != 0) {
        std::cout << "[OP_SW -> STUB] Write while cache is isolated." << '\n';
        return;
    } 

    std::uint32_t imm_se = instruction.imm_se();
    std::uint32_t rt = instruction.rt();
    std::uint32_t rs = instruction.rs();

    std::uint32_t addr = getReg(rs) + imm_se;
    auto value = getReg(rt);

    this->write<std::uint32_t>(addr, value);
}

void CPU::OP_SLL(Instruction &instruction) {
    std::uint32_t sa = instruction.shamt();
    std::uint32_t rt = instruction.rt();
    std::uint32_t rd = instruction.rd();

    std::uint32_t value = getReg(rt) << sa;
    setReg(rd, value);
}


void CPU::OP_ADDIU(Instruction &instruction) {
    std::uint32_t imm = instruction.imm_se();
    std::uint32_t rt = instruction.rt();
    std::uint32_t rs = instruction.rs();

    std::uint32_t value = getReg(rs) + imm;
    setReg(rt, value);
}

void CPU::OP_J(Instruction &instruction) {
    std::uint32_t target = instruction.target();
    
    this->delayed_branch = ((this->pc + 4) & 0xF0000000) | (target << 2);
}

void CPU::OP_OR(Instruction &instruction) {
    std::uint32_t rt = instruction.rt();
    std::uint32_t rs = instruction.rs();
    std::uint32_t rd = instruction.rd();
    
    std::uint32_t value = this->getReg(rs) | this->getReg(rt);
    this->setReg(rd, value);
}

void CPU::OP_COP0(Instruction &instruction) {
    switch(instruction.rs()) {
        case 0x4:
            this->MTC0(instruction);
            break;
        default:
            std::cerr << "[CPU] Unhandled COP0 instruction: 0x" << std::hex << instruction.rs() << std::endl;
            throw std::exception();
    }
}

void CPU::MTC0(Instruction& instruction) {
    std::uint32_t rd = instruction.rd();
    std::uint32_t rt = instruction.rt();

    std::uint32_t value = getReg(rt);

    switch(rd) {
        case 0xC:
            this->sr = value;
            break;
        default:
            std::cerr << "[CPU] Unhandled COP0 register: 0x" << std::hex << rd << std::endl;
            throw std::exception();
    }
}