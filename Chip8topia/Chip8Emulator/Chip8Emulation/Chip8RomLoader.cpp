#include "Chip8RomLoader.h"

#include <stdexcept>

auto Chip8RomLoader::loadRom(const char* romPath) -> std::vector<uint8> {
    auto rom = readRom(romPath);
    if (!checkRom(rom))
    {
        throw std::runtime_error("Invalid rom");
    }
    return rom;
}

auto Chip8RomLoader::readRom(const char* romPath) -> std::vector<uint8> {
    return std::vector<uint8>();
}

auto Chip8RomLoader::checkRom(const std::vector<uint8>& rom) -> bool {
    return false;
}

void Chip8RomLoader::printRomInfo(const std::vector<uint8>& rom) {
}
