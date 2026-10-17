#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

std::uint64_t constexpr RAM_SIZE = 4 * 1024 * 1024;

class RAM {
public:
    RAM() : ramData(RAM_SIZE) {}
    std::vector<std::uint8_t> ramData{};
    template <typename T> T read(std::uint32_t offset) {
        T val = 0;

        for (std::size_t i = 0; i < sizeof(T); ++i) {
            val |= static_cast<T>(ramData[offset + i]) << (i * 8);
        }

        return val;
    }

    template <typename T>
    void write(std::uint32_t offset, T val) {
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            ramData[offset + i] = static_cast<std::uint8_t>(val >> (i * 8));
        }
    }
};