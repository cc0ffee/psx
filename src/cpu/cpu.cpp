#include "../include/psx/cpu.hpp"
#include <iostream>

void CPU::run() {
    current_pc = pc;
    const std::uint32_t instruction = bus.read<std::uint32_t>(pc);

    pc = next_pc;
    next_pc += 4;

    decodeExec(instruction);
}

void CPU::decodeExec(std::uint32_t instruction) {
    std::cerr << "[CPU] Unhandled Instruction: " << std::hex << instruction << std::endl;
    throw std::exception();
}

template <typename T> T CPU::read(std::uint32_t addr) {
    return this->bus.read<T>(addr);
}