#pragma once

#include <array>
#include <IconsFontAwesome6.h>

// #include "../ImGuiHelper/ImGuiHelper.h"

class Chip8Emulator;
class Chip8topiaSoundUi
{
private:
#if defined(BUILD_RELEASE)
    static constexpr bool INITIAL_WINDOW_STATE = false;
#else
    static constexpr bool INITIAL_WINDOW_STATE = true;
#endif

public:
    Chip8topiaSoundUi() = default;
    Chip8topiaSoundUi(const Chip8topiaSoundUi&) = delete;
    Chip8topiaSoundUi(Chip8topiaSoundUi&&) = delete;
    auto operator=(const Chip8topiaSoundUi&) -> Chip8topiaSoundUi& = delete;
    auto operator=(Chip8topiaSoundUi&&) -> Chip8topiaSoundUi& = delete;
    ~Chip8topiaSoundUi() = default;

public:
    void drawSoundMenu(Chip8Emulator& emulator);
    void drawSoundWindows(Chip8Emulator& emulator);
    void closeAllWindows();

private:
    void drawSoundWaveType(Chip8Emulator& emulator);
    void drawSoundState(Chip8Emulator& emulator);
    void drawSoundFrequency(Chip8Emulator& emulator);
    void drawSoundVolume(Chip8Emulator& emulator);

private:
    bool m_isSoundStateWindowOpen = INITIAL_WINDOW_STATE;
};
