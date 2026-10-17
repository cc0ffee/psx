#include "../include/psx/cpu.hpp"
#include <iostream>

void CPU::run() {
    Instruction instruction(bus.read<std::uint32_t>(pc));

    const std::uint32_t current_pc = pc;
    const std::uint32_t next_pc = delayed_branch.value_or(current_pc + 4);

    delayed_branch.reset();
    pc = next_pc;

    outRegs = gRegs;

    setReg(load.first, load.second);
    load = {0, 0};

    decodeExec(instruction);

    gRegs = outRegs;
    gRegs[0] = 0;
}

void CPU::decodeExec(Instruction& instruction) {
    switch(instruction.op()) {
        case 0x00: {
            switch(instruction.funct()) {
                case 0x00:
                    this->OP_SLL(instruction);
                    break;
                case 0x08:
                    this->OP_JR(instruction);
                    break;
                case 0x09:
                    this->OP_JALR(instruction);
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
        case 0x01:
            this->OP_BCONDZ(instruction);
            break;
        case 0x02:
            this->OP_J(instruction);
            break;
case 0x03:
            this->OP_JAL(instruction);
            break;
        case 0x04:
            this->OP_BEQ(instruction);
            break;
        case 0x05:
            this->OP_BNE(instruction);
            break;
case 0x06:
            this->OP_BLEZ(instruction);
            break;
        case 0x07:
            this->OP_BGTZ(instruction);
            break;
        case 0x08:
            this->OP_ADDI(instruction);
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
    this->outRegs[reg] = value;
    this->outRegs[0] = 0;
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

void CPU::OP_BNE(Instruction& instruction) {
    std::uint32_t imm_se = instruction.imm_se();
    std::uint32_t rt = instruction.rt();
    std::uint32_t rs = instruction.rs();

    if (this->getReg(rs) != this->getReg(rt)) {
        this->branch(imm_se);
    }
}

void CPU::OP_BEQ(Instruction& instruction) {
    std::uint32_t imm_se = instruction.imm_se();
    std::uint32_t rt = instruction.rt();
    std::uint32_t rs = instruction.rs();

    if (this->getReg(rs) == this->getReg(rt)) {
        this->branch(imm_se);
    }
}

void CPU::OP_BGTZ(Instruction& instruction) {
    const std::uint32_t imm_se = instruction.imm_se();
    const std::uint32_t rs = instruction.rs();

    const auto value = static_cast<std::int32_t>(getReg(rs));

    if (value > 0) {
        branch(imm_se);
    }
}

void CPU::OP_BLEZ(Instruction& instruction) {
    const std::uint32_t imm_se = instruction.imm_se();
    const std::uint32_t rs = instruction.rs();

    const auto value = static_cast<std::int32_t>(getReg(rs));

    if (value <= 0) {
        branch(imm_se);
    }
}

void CPU::branch(std::uint32_t offset) {
    delayed_branch = pc + (offset << 2);
}
void CPU::OP_JAL(Instruction& instruction) {
    setReg(31, pc + 4);
    OP_J(instruction);
}
void CPU::OP_JR(Instruction &instruction) {
    std::uint32_t rs = instruction.rs();

    this->delayed_branch = this->getReg(rs);
}

void CPU::OP_JALR(Instruction& instruction) {
    const std::uint32_t rs = instruction.rs();
    const std::uint32_t rd = instruction.rd();

    const std::uint32_t target = getReg(rs);
    const std::uint32_t ra = this->pc + 4;

    setReg(rd, ra);
    delayed_branch = target;
}

void CPU::OP_BCONDZ(Instruction& instruction) {
    const std::uint32_t imm_se = instruction.imm_se();
    const std::uint32_t rs = instruction.rs();
    const std::uint32_t rt = instruction.rt();

    const std::int32_t value = static_cast<std::int32_t>(getReg(rs));

    const bool is_bgez = (rt & 1u) != 0;
    const bool is_link = (rt & 0x10u) != 0;

    const bool test = (value >= 0) == is_bgez;

    if (is_link) {
        setReg(31, pc + 4);
    }

    if (test) {
        branch(imm_se);
    }
}

