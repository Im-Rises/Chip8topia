#pragma once

#include "Chip8Core/Chip8Core.h"
#include "Chip8Emulation/Chip8VideoEmulation.h"
#include "Chip8Emulation/Chip8SoundEmulation.h"
#include "Chip8Emulation/Chip8RomLoader.h"

// TODO: Faire un switch case qui démarre la version de Core sélectionné avec la création de Chip8Core m_core; en tant que unique_ptr
// TODO: Pour la partie emulation faire des surcharges de fonctions avec comme paramètre const SuperChip8Core& core, const Chip8Core& core, const XOChip8Core& core, de cette façon on pourra définir trois fonctions différentes pour chaque version de Core et le prendra autmaotiquement lors de l'appel ?

class Chip8Emulator {
public:
    Chip8Emulator();
    Chip8Emulator(const Chip8Emulator&) = delete;
    Chip8Emulator(Chip8Emulator&&) = delete;
    auto operator=(const Chip8Emulator&) -> Chip8Emulator& = delete;
    auto operator=(Chip8Emulator&&) -> Chip8Emulator& = delete;
    ~Chip8Emulator();

public:
    void restart();
    void loadRom(const std::string& romPath);

    void update(const float deltaTime);
    void render();

    void togglePause();

    void setIsTurboMode(const bool isTurboMode);
    [[nodiscard]] auto getIsPaused() const -> bool;
    [[nodiscard]] auto getChip8Core() -> Chip8Core*;
    [[nodiscard]] auto getChip8VideoEmulation() -> Chip8VideoEmulation&;

private:
    void OnInput(const uint8 key, const bool isPressed);

private:
    Chip8Core m_core;
    Chip8VideoEmulation m_videoEmulation;
    //    Chip8SoundEmulation m_soundEmulation;
    //    Chip8RomLoader m_romLoader;

    bool m_isRomLoaded = false;
    bool m_isTurboMode = false;
    bool m_isPaused = false;

    float m_accumulator = 0.0F;
};
