#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

std::uint64_t constexpr BIOS_SIZE = 512 * 1024;

class Bios {
public:
    Bios(const char* path) : biosPath(path) {
        std::ifstream biosFile(biosPath, std::ios::binary | std::ios::ate);

        if (!biosFile) throw std::runtime_error("[BIOS] Failed to get BIOS file loaded.");

        // in general, the PSX bios file is 512KB. 
        // There are some exceptions this, including the ability to load a custom bios.
        // thus the implementation accounts for different file sizes.
        auto size = biosFile.tellg();
        if (size < 0) throw std::runtime_error("[BIOS] Failed to get the size of BIOS file.");
        if (size != BIOS_SIZE) isRegularBios = false;

        biosData.resize(size);
        biosFile.seekg(0, std::ios::beg);

        if (!biosData.empty() && !biosFile.read(reinterpret_cast<char*>(biosData.data()), size))
            throw std::runtime_error("[BIOS] Failed to read bytes from BIOS file.");

    }
    std::vector<std::uint8_t> biosData{};
    bool isRegularBios{true};
    std::string biosPath;
    template <typename T> T read(std::uint32_t offset) {
        T val = 0;

        for (std::size_t i = 0; i < sizeof(T); ++i) {
            val |= static_cast<T>(biosData[offset + i]) << (i * 8);
        }

        return val;
    }
};