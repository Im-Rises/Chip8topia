#pragma once

class Chip8topia;
class Chip8EmulationUi {
public:
    Chip8EmulationUi() = default;
    Chip8EmulationUi(const Chip8EmulationUi&) = delete;
    Chip8EmulationUi(Chip8EmulationUi&&) = delete;
    auto operator=(const Chip8EmulationUi&) -> Chip8EmulationUi& = delete;
    auto operator=(Chip8EmulationUi&&) -> Chip8EmulationUi& = delete;
    ~Chip8EmulationUi() = default;

public:
    void drawEmulationMenu(Chip8topia& chip8topia);
};
