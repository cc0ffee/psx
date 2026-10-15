#pragma once
#include "psx/bios.hpp"
#include "psx/range.hpp"
#include <iostream>
class Bus {
public:
    Bus(Bios& bios) : bios(bios) { }
    Bios& bios;
    template <typename T> T read(std::uint32_t addr) {
        if (auto offset = map::BIOS.contains(addr); offset != UINT32_MAX) {
            return this->bios.read<T>(offset);
        }
        std::cerr << "[BUS] Unhandled read at address: 0x" << std::hex << addr << std::endl;
        throw std::exception();
    }

    template <typename T> void write(std::uint32_t addr, T val) {
        std::cerr << "[BUS] Unhandled write at address: 0x" << std::hex << addr << std::endl;
        throw std::exception();
    }
private:
};