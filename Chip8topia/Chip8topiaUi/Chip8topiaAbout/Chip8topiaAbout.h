#pragma once

#include <array>
#include <string>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8topiaAbout
{
public:
    Chip8topiaAbout();
    Chip8topiaAbout(const Chip8topiaAbout&) = delete;
    Chip8topiaAbout(Chip8topiaAbout&&) = delete;
    auto operator=(const Chip8topiaAbout&) -> Chip8topiaAbout& = delete;
    auto operator=(Chip8topiaAbout&&) -> Chip8topiaAbout& = delete;
    ~Chip8topiaAbout() = default;

public:
    void drawAboutMenu();
    void drawAboutWindows();

private:
    void drawAboutChip8topia();
    void drawAboutChip8();

private:
    std::array<ImGuiMenuItemPopup<>, 2> m_menuItem;
};
