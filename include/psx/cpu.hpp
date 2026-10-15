#pragma once
#include "psx/bus.hpp"
#include <cstdint>
#include <array>

class CPU {
public:
    CPU(Bus& bus) : bus(bus) {};
    void run();
private:
    std::array<std::uint32_t, 32> gRegs{};
    std::uint32_t pc{0xBFC00000};
    std::uint32_t next_pc{0xBFC00004};
    std::uint32_t current_pc{};
    std::uint32_t hi{};
    std::uint32_t lo{};
    Bus& bus;
    void decodeExec(std::uint32_t instruction);
    template <typename T> T read(std::uint32_t addr);
};