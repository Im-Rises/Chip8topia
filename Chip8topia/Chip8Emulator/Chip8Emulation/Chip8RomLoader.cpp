#include "Chip8RomLoader.h"

#include <fstream>

#include "../Chip8Core/Core/Cpu.h"

auto Chip8RomLoader::loadRom(const std::string& romPath) -> std::vector<uint8> {
    std::ifstream romFile(romPath, std::ios::binary);
    checkRomFileSize(romFile);
    std::vector<uint8> romData = readRom(romFile);
    romFile.close();

    // TODO: Add std::filesystem::file_size() to checkRomFileSize() and file exists check
    //    std::filesystem::path filePath = "path/to/file.txt";
    //    if (std::filesystem::exists(filePath)) {
    //        std::cout << "File size: " << std::filesystem::file_size(filePath) << " bytes\n";
    //    }

    return romData;
}

auto Chip8RomLoader::checkRomFileSize(std::ifstream& romFile) -> bool {
    romFile.seekg(0, std::ios::end);
    const auto fileSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);
    return fileSize == Cpu::ROM_SIZE;
}

auto Chip8RomLoader::readRom(std::ifstream& romFile) -> std::vector<uint8> {
    std::vector<uint8> rom;
    rom.resize(Cpu::ROM_SIZE);
    romFile.read(reinterpret_cast<char*>(rom.data()), Cpu::ROM_SIZE);
    return rom;
}
