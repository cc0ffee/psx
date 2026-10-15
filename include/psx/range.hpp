#pragma once
#include <cstdint>

struct Range {
    std::uint32_t start;
    std::uint32_t length;

    std::uint32_t contains(std::uint32_t addr) const {
        return (addr >= start && addr < start + length) ? addr - start : UINT32_MAX;
    }
};

namespace map {
    constexpr Range BIOS{0xBFC00000, 512 * 1024};
}