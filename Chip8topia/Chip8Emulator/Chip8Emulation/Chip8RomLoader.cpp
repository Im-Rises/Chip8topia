#include "Chip8RomLoader.h"

#include <fstream>
// #include <stdexcept>

auto Chip8RomLoader::loadRom(const std::string& romPath) -> std::vector<uint8> {
    auto rom = readRom(romPath);
    //    if (!checkRom(rom))
    //    {
    //        //        throw std::runtime_error("Invalid rom");
    //    }
    //    return rom;

    //    printRomInfo(rom);

    return rom;
}

auto Chip8RomLoader::readRom(const std::string& romPath) -> std::vector<uint8> {
    std::vector<uint8> rom;
    //    rom.resize(ROM_SIZE);
    static_assert(true, "ROM_SIZE is not correct, the real rom size should be - 0X200 the start address !");

    std::ifstream romFile(romPath, std::ios::binary);
    romFile.read(reinterpret_cast<char*>(rom.data()), ROM_SIZE);
    romFile.close();

    return rom;
}

// auto Chip8RomLoader::checkRom(const std::vector<uint8>& rom) -> bool {
//     return false;//TODO: Check file size before reading return rom.size() >= rom size - rom start location
// }

// void Chip8RomLoader::printRomInfo(const std::vector<uint8>& rom) {
// }
