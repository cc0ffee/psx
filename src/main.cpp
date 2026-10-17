#include "psx/bios.hpp"
#include "psx/bus.hpp"
#include "psx/cpu.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: psx.exe [BIOS Path]" << std::endl;
        return 1;
    }

    Bios bios(argv[1]);
    RAM ram;
    Bus bus(bios, ram);
    CPU cpu(bus);
    std::cout << "Loaded Bios from Path: " << bios.biosPath << '\n';
    std::cout << "Is Valid (512KB): " << bios.isRegularBios << '\n';
    std::cout << "First instruction: " << std::hex << bios.read<std::uint32_t>(0x0) << std::endl;

    while (true) {
        cpu.run();
    }

    return 0;
}