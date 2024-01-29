#pragma once

// TODO: Do check to verify that the rom is valid
//  Read rom to a buffer

// TODO: use std::move and std::forward and references

// TODO: use an exception to handle errors // #include <stdexcept>

// TODO: use std::filesystem::path

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
    auto loadRom(const std::string& romPath) -> std::vector<uint8>;

private:
    auto checkRomFileSize(std::ifstream& romFile) -> bool;
    auto readRom(std::ifstream& romFile) -> std::vector<uint8>;
};
