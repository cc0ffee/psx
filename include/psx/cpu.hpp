#pragma once
#include "psx/bus.hpp"
#include "psx/instruction.hpp"
#include <cstdint>
#include <array>

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
    std::array<std::uint32_t, 32> gRegs{0};
    std::uint32_t pc{0xBFC00000};
    std::uint32_t next_pc{0xBFC00004};
    std::uint32_t current_pc{};
    std::uint32_t hi{};
    std::uint32_t lo{};
    Bus& bus;
    void decodeExec(Instruction& instruction);
    template <typename T> T read(std::uint32_t addr) {
        return this->bus.read<T>(addr);
    }

    std::uint32_t getReg(std::uint32_t reg);
    void setReg(std::uint32_t reg, const std::uint32_t &value);

    void OP_ORI(Instruction& instruction);
    void OP_LUI(Instruction& instruction);
};