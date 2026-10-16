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
        if (auto offset = map::MEMCONTROL.contains(addr); offset != UINT32_MAX) {
            switch (offset) {
                case 0x0: {
                    if (val != 0x1f000000) {
                        std::cerr << "[BUS] BAD Expansion 1 base address: 0x" << std::hex << val << std::endl;
                        throw std::exception();
                    }
                    return;
                }
                case 0x4: {
                    if (val != 0x1f802000) {
                        std::cerr << "[BUS] BAD Expansion 2 base address: 0x" << std::hex << val << std::endl;
                        throw std::exception();
                    }
                    return;
                }
                default:
                    std::cout << "[BUS] Unhandled write to MEMCONTROL Register." << '\n';
                    return;
            }
        } else if (auto offset = map::DRAM_CTRL.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS] STUB: Unhandled write to DRAM_CTRL Register." << '\n';
            return;
        }
        std::cerr << "[BUS] Unhandled write at address: 0x" << std::hex << val << " @ 0x" << addr << std::endl;
        throw std::exception();
    }
private:
};