#include "psx/bios.hpp"
#include "psx/bus.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: psx.exe [BIOS Path]" << std::endl;
        return 1;
    }

    Bios bios(argv[1]);
    
    std::cout << "Loaded Bios from Path: " << bios.biosPath << '\n';
    std::cout << "Is Valid (512KB): " << bios.isRegularBios << '\n';
    std::cout << "First instruction: " << std::hex << bios.load<std::uint32_t>(0x0) << std::endl;

    return 0;
}