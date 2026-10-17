#pragma once
#include "psx/bus.hpp"
#include "psx/instruction.hpp"
#include <cstdint>
#include <array>
#include <utility>
#include <optional>

class CPU {
public:
    CPU(Bus& bus) : bus(bus) {
        for (auto& reg : gRegs) {
            reg = 0xDEADBEEF;
        }
        this->gRegs[0] = 0;
    };
    void run();
private:
    std::array<std::uint32_t, 32> gRegs{};
    std::array<std::uint32_t, 32> outRegs{};
    std::uint32_t pc{0xBFC00000};
    std::uint32_t next_pc{0xBFC00004};
    std::uint32_t current_pc{};
    std::optional<std::uint32_t> delayed_branch{};
    std::pair<std::uint32_t, std::uint32_t> load = {0, 0};
    std::uint32_t hi{0xDEADBEEF};
    std::uint32_t lo{0xDEADBEEF};

    std::uint32_t bpc{0};
    std::uint32_t bda{0};
    std::uint32_t tar{0};
    std::uint32_t bdam{0};
    std::uint32_t bpcm{0};
    std::uint32_t dcic{0};
    std::uint32_t sr{0};
    std::uint32_t epc{0};

    Bus& bus;
    void decodeExec(Instruction& instruction);
    template <typename T> T read(std::uint32_t addr) {
        return this->bus.read<T>(addr);
    }
    template <typename T> void write(std::uint32_t addr, T value) {
        this->bus.write<T>(addr, value);
    }

    std::uint32_t getReg(std::uint32_t reg);
    void setReg(std::uint32_t reg, const std::uint32_t &value);

    void OP_SLL(Instruction& instruction);
    void OP_SRL(Instruction& instruction);
    void OP_SRA(Instruction& instruction);
    void OP_SLLV(Instruction& instruction);
    void OP_SRLV(Instruction& instruction);
    void OP_SRAV(Instruction& instruction);
    void OP_JR(Instruction& instruction);
    void OP_JALR(Instruction& instruction);
    void OP_SYSCALL(Instruction& instruction);
    void OP_BREAK(Instruction& instruction);
    void OP_MFHI(Instruction& instruction);
    void OP_MTHI(Instruction& instruction);
    void OP_MFLO(Instruction& instruction);
    void OP_MTLO(Instruction& instruction);
    void OP_MULT(Instruction& instruction);
    void OP_MULTU(Instruction& instruction);
    void OP_DIV(Instruction& instruction);
    void OP_DIVU(Instruction& instruction);
    void OP_ADD(Instruction& instruction);
    void OP_ADDU(Instruction& instruction);
    void OP_SUB(Instruction& instruction);
    void OP_SUBU(Instruction& instruction);
    void OP_AND(Instruction& instruction);
    void OP_OR(Instruction& instruction);
    void OP_XOR(Instruction& instruction);
    void OP_NOR(Instruction& instruction);
    void OP_SLT(Instruction& instruction);
    void OP_SLTU(Instruction& instruction);

    void OP_BCONDZ(Instruction& instruction);
    void OP_J(Instruction& instruction);
    void OP_JAL(Instruction& instruction);
    void OP_BEQ(Instruction& instruction);
    void OP_BNE(Instruction& instruction);
    void OP_BLEZ(Instruction& instruction);
    void OP_BGTZ(Instruction& instruction);
    void OP_ADDI(Instruction& instruction);
    void OP_ADDIU(Instruction& instruction);
    void OP_SLTI(Instruction& instruction);
    void OP_SLTIU(Instruction& instruction);
    void OP_ANDI(Instruction& instruction);
    void OP_ORI(Instruction& instruction);
    void OP_XORI(Instruction& instruction);
    void OP_LUI(Instruction& instruction);
    void OP_COP0(Instruction& instruction);
    void OP_COP1(Instruction& instruction);
    void OP_COP2(Instruction& instruction);
    void OP_COP3(Instruction& instruction);
    void OP_LB(Instruction& instruction);
    void OP_LH(Instruction& instruction);
    void OP_LWL(Instruction& instruction);
    void OP_LW(Instruction& instruction);
    void OP_LBU(Instruction& instruction);
    void OP_LHU(Instruction& instruction);
    void OP_LWR(Instruction& instruction);
    void OP_SB(Instruction& instruction);
    void OP_SH(Instruction& instruction);
    void OP_SWL(Instruction& instruction);
    void OP_SW(Instruction& instruction);
    void OP_SWR(Instruction& instruction);
    void OP_LWC0(Instruction& instruction);
    void OP_LWC1(Instruction& instruction);
    void OP_LWC2(Instruction& instruction);
    void OP_LWC3(Instruction& instruction);
    void OP_SWC0(Instruction& instruction);
    void OP_SWC1(Instruction& instruction);
    void OP_SWC2(Instruction& instruction);
    void OP_SWC3(Instruction& instruction);

    void MTC0(Instruction& instruction);
    void MFC0(Instruction& instruction);
    void branch(std::uint32_t offset);
};