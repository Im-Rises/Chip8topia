#pragma once

#include <binaryLib/binaryLib.h>
#include <vector>
#include <string>

class Chip8RomLoader {
public:
    Chip8RomLoader() = default;
    Chip8RomLoader(const Chip8RomLoader&) = delete;
    Chip8RomLoader(Chip8RomLoader&&) = delete;
    auto operator=(const Chip8RomLoader&) -> Chip8RomLoader& = delete;
    auto operator=(Chip8RomLoader&&) -> Chip8RomLoader& = delete;
    ~Chip8RomLoader() = default;

public:
    static auto loadRomFromPath(const std::string& romPath) -> std::vector<uint8>;
    static auto loadRomFromData(const std::string_view& romBuffer) -> std::vector<uint8>;

private:
    static auto checkFileExtension(const std::string& romPath) -> bool;
    static auto checkFileExists(const std::string& romPath) -> bool;
    static auto checkRomFileSize(const std::string& romPath) -> bool;
    static auto readRom(const std::string& romPath) -> std::vector<uint8>;
};
