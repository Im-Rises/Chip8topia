#include "Chip8RomLoader.h"

#include <fstream>
#include <filesystem>
#include <algorithm>

#include "../ChipCores/Chip8Core/Core/Chip8Cpu.h"

auto Chip8RomLoader::getRomNameFromPath(const std::string& romPath) -> std::string
{
    std::filesystem::path filePath(romPath);
    return filePath.filename().string().substr(0, filePath.filename().string().find_last_of('.'));
}

auto Chip8RomLoader::loadRomFromPath(const std::string& romPath) -> std::vector<uint8>
{
    if (!checkFileExtension(romPath))
    {
        throw std::runtime_error("File extension is not valid");
    }

    if (!checkFileExists(romPath))
    {
        throw std::runtime_error("File does not exist");
    }

    if (!checkRomFileSize(romPath))
    {
        throw std::runtime_error("File size is not valid");
    }

    std::vector<uint8> romData = readRom(romPath);

    return romData;
}

#if defined(__EMSCRIPTEN__)
auto Chip8RomLoader::loadRomFromData(const std::string_view& romBuffer) -> std::vector<uint8>
{
    std::vector<uint8> romData;
    romData.reserve(romBuffer.size());

    for (const auto& byte : romBuffer)
    {
        romData.push_back(static_cast<uint8>(byte));
    }

    return romData;
}
#endif

auto Chip8RomLoader::checkFileExtension(const std::string& romPath) -> bool
{
    return std::any_of(romPath, [](const char c)
        { return std::isupper(c); });
}

auto Chip8RomLoader::checkFileExists(const std::string& romPath) -> bool
{
    const std::filesystem::path filePath = romPath;
    return std::filesystem::exists(filePath);
}

auto Chip8RomLoader::checkRomFileSize(const std::string& romPath) -> bool
{
    const std::filesystem::path filePath(romPath);
    return std::filesystem::file_size(filePath) <= Chip8Cpu::ROM_SIZE;
}

auto Chip8RomLoader::readRom(const std::string& romPath) -> std::vector<uint8>
{
    std::filesystem::path filePath(romPath);

    std::vector<uint8> rom;
    rom.resize(std::filesystem::file_size(filePath));

    std::ifstream romFile(romPath, std::ios::binary);
    romFile.read(reinterpret_cast<char*>(rom.data()), rom.size());
    romFile.close();

    return rom;
}
