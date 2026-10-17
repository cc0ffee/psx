#pragma once
#include "psx/bios.hpp"
#include "psx/range.hpp"
#include "psx/ram.hpp"
#include <iostream>
#include <array>

const std::array<std::uint32_t, 8> REGION_MASK = {
    // KUSEG: 2048MB
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    // KSEG0: 512MB
    0x7fffffff,
    // KSEG1: 512MB
    0x1fffffff,
    // KSEG2: 1024MB
    0xffffffff, 0xffffffff 
};

inline std::uint32_t mask_region(std::uint32_t addr) {
    auto idx = (addr >> 29);
    return addr & REGION_MASK[idx];
}

class Bus {
public:
    Bus(Bios& bios, RAM& ram) : bios(bios), ram(ram) { }
    Bios& bios;
    RAM& ram;
    template <typename T> T read(std::uint32_t addr) {
        addr = mask_region(addr);
        if (auto offset = map::BIOS.contains(addr); offset != UINT32_MAX) {
            return this->bios.read<T>(offset);
        } else if (auto offset = map::RAM.contains(addr); offset != UINT32_MAX) {
            return this->ram.read<T>(offset);
        } else if (auto offset = map::EXPANSION_1.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled read to Expansion 1 Register." << '\n';
            return 0;
        } else if (auto offset = map::IRQ_CTRL.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandledread to IRQ_CTRL Register: 0x" << std::hex << offset << '\n';
            return 0;
        }
        std::cerr << "[BUS] Unhandled read at address: 0x" << std::hex << addr << std::endl;
        throw std::exception();
    }

    template <typename T> void write(std::uint32_t addr, T val) {
        addr = mask_region(addr);
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
            std::cout << "[BUS -> STUB]: Unhandled write to DRAMs_CTRL Register." << '\n';
            return;
        } else if (auto offset = map::CACHE_CTRL.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled write to CACHE_CTRL Register." << '\n';
            return;
        } else if (auto offset = map::RAM.contains(addr); offset != UINT32_MAX) {
            this->ram.write<T>(offset, val);
            return;
        } else if (auto offset = map::SPU.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled write to SPU Register." << '\n';
            return;
        } else if (auto offset = map::EXPANSION_2.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled write to Expansion 2 Register." << '\n';
            return;
        } else if (auto offset = map::IRQ_CTRL.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled write to IRQ_CTRL Register: 0x" << std::hex << offset << " <- 0x" << val << '\n';
            return;
        } else if (auto offset = map::TIMERS.contains(addr); offset != UINT32_MAX) {
            std::cout << "[BUS -> STUB]: Unhandled write to TIMERS Register: 0x" << std::hex << offset << " <- 0x" << val << '\n';
            return;
        }
        std::cerr << "[BUS] Unhandled write at address: 0x" << std::hex << val << " @ 0x" << addr << std::endl;
        throw std::exception();
    }
private:
};