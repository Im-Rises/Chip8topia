#pragma once

#include <array>
#include <string>

#include "../ImGuiHelper/ImGuiHelper.h"

class Chip8About {
public:
    Chip8About();
    Chip8About(const Chip8About&) = delete;
    Chip8About(Chip8About&&) = delete;
    auto operator=(const Chip8About&) -> Chip8About& = delete;
    auto operator=(Chip8About&&) -> Chip8About& = delete;
    ~Chip8About() = default;

public:
    void drawAboutMenu();
    void drawAboutWindows();

private:
    //    void drawAboutPopUpInternal(const std::string_view& popupName, const std::function<void()>& drawAboutPopUpContent, bool& showAboutPopup);

    void drawAboutChip8topia();
    void drawAboutChip8();

private:
    std::array<ImGuiMenuItemPopup<>, 2> m_menuItem;
};
