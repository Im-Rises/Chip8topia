#include "Chip8RomLoader.h"

#include <fstream>
#include <filesystem>

#include "../Chip8Core/Core/Cpu.h"

auto Chip8RomLoader::loadRom(const std::string& romPath) -> std::vector<uint8> {
    if (!checkFileExists(romPath))
    {
        throw std::runtime_error("File does not exist");
    }

    // TODO: Check if rom size is between 0 and 4096
    //    if (!checkRomFileSize(romPath))
    //    {
    //        throw std::runtime_error("File size is not valid");
    //    }

    std::ifstream romFile(romPath, std::ios::binary);
    std::vector<uint8> romData = readRom(romFile);
    romFile.close();

    return romData;
}

auto Chip8RomLoader::checkFileExists(const std::string& romPath) -> bool {
    const std::filesystem::path filePath = romPath;
    return std::filesystem::exists(filePath);
}

auto Chip8RomLoader::checkRomFileSize(const std::string& romPath) -> bool {
    const std::filesystem::path filePath = romPath;
    return std::filesystem::file_size(filePath) == Cpu::ROM_SIZE;
}

auto Chip8RomLoader::readRom(std::ifstream& romFile) -> std::vector<uint8> {
    std::vector<uint8> rom;
    rom.resize(Cpu::ROM_SIZE);
    romFile.read(reinterpret_cast<char*>(rom.data()), Cpu::ROM_SIZE);
    return rom;
}
