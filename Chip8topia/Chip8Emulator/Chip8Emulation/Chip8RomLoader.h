#pragma once

// TODO: Do check to verify that the rom is valid
//  Read rom to a buffer

// TODO: use std::move and std::forward and references

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
    auto readRom(const std::string& romPath) -> std::vector<uint8>;
    //    auto checkRom(const std::vector<uint8>& rom) -> bool;

    //    void printRomInfo(const std::vector<uint8>& rom);

private:
    static constexpr size_t ROM_START = 0x200;
    static constexpr size_t ROM_SIZE = 0x1000;
};