#include "Chip8topiaUi.h"

#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
// #include <format>
#include <fmt/format.h>
#include <iostream>
#if defined(__EMSCRIPTEN__)
#include <emscripten_browser_file.h>
#endif

#include "../Chip8topia.h"

#if defined(__EMSCRIPTEN__)
void handle_upload_file(std::string const& filename, std::string const& mime_type, std::string_view buffer, void* chip8emulator) {
    std::cout << "File uploaded: " << filename << " (" << mime_type << ")" << '\n';
    Chip8Emulator* chip8Emulator = static_cast<Chip8Emulator*>(chip8emulator);

    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromData(buffer);
        chip8Emulator->loadRom(rom);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
#endif

Chip8topiaUi::Chip8topiaUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.subscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_ToggleWindowsVisibilityEvent.subscribe(this, &Chip8topiaUi::toggleWindowsVisibility);
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.subscribe(this, &Chip8topiaUi::openRomWindow);
#endif
}

Chip8topiaUi::~Chip8topiaUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_ToggleMainBarEvent.unsubscribe(this, &Chip8topiaUi::toggleMenuBarVisibility);
    Chip8topiaInputHandler::getInstance().m_ToggleWindowsVisibilityEvent.unsubscribe(this, &Chip8topiaUi::toggleWindowsVisibility);
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.unsubscribe(this, &Chip8topiaUi::openRomWindow);
#endif
}

void Chip8topiaUi::drawUi(Chip8topia& chip8topia) {
    drawMainMenuBar(chip8topia);
}

void Chip8topiaUi::drawMainMenuBar(Chip8topia& chip8topia) {
    if (m_isMenuBarOpen && ImGui::BeginMainMenuBar())
    {
        drawFileMenu(chip8topia);
        drawViewMenu(chip8topia);
        drawEngineEmulationMenu(chip8topia);
        m_chip8VideoUi.drawVideoMenu();
        m_chip8topiaDebugger.drawDebuggerMenu();
        m_chip8About.drawAboutMenu();

        ImGui::EndMainMenuBar();
    }

    if (m_windowsVisible)
    {
        m_chip8VideoUi.drawVideoWindows(chip8topia.getChip8Emulator());
        m_chip8topiaDebugger.drawDebuggerWindows(chip8topia.getChip8Emulator());
    }

    drawRomWindow(chip8topia);
    m_chip8About.drawAboutWindows();

#if !defined(BUILD_RELEASE)
    ImGui::ShowDemoWindow();
#endif
}

void Chip8topiaUi::drawFileMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("File"))
    {
#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem("Open integrated rom..."))
#else
        if (ImGui::MenuItem("Open rom...", "Ctrl+O"))
#endif
        {
            openRomWindow();
        }

#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem("Open rom"))
        {
            //            emscripten_browser_file::upload(CHIP8_ROM_FILE_EXTENSION, handle_upload_file);
            emscripten_browser_file::upload(CHIP8_ROM_FILE_EXTENSION, handle_upload_file, &chip8topia.getChip8Emulator());
        }
#endif

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            chip8topia.closeRequest();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawEngineEmulationMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("Engine/Emulation"))
    {
        if (ImGui::MenuItem(fmt::format("Toggle Engine turbo mode : {}", chip8topia.getIsTurboMode() ? "ON " : "OFF").c_str(), "F3"))
        {
            chip8topia.toggleTurboMode();
        }

        if (ImGui::MenuItem(chip8topia.getChip8Emulator().getIsPaused() ? "Resume" : "Pause", "P"))
        {
            Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.trigger();
        }

        if (ImGui::MenuItem("Restart", "L"))
        {
            Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.trigger();
        }

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::drawViewMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("View"))
    {
        // TODO: Create a dictonary of the keys and the name of the menu item with the event, to be sure to call the right event
        ImGui::MenuItem("Show/Hide MenuBar", "F1", &m_isMenuBarOpen);

        ImGui::MenuItem("Show/Hide Windows", "F2", &m_windowsVisible);

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem("Center window", "F10"))
        {
            chip8topia.centerWindow();
        }

        if (ImGui::MenuItem("FullScreen", "F11"))
        {
            chip8topia.toggleFullScreen();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaUi::openRomWindow() {
    IGFD::FileDialogConfig config;
    config.path = DEFAULT_FOLDER_PATH;
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", CHIP8_ROM_FILE_EXTENSION, config);
}

void Chip8topiaUi::drawRomWindow(Chip8topia& chip8topia) {
    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

            try
            {
                std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(filePathName);
                chip8topia.getChip8Emulator().loadRom(rom);
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Chip8topiaUi::toggleMenuBarVisibility() {
    m_isMenuBarOpen = !m_isMenuBarOpen;
}

void Chip8topiaUi::toggleWindowsVisibility() {
    m_windowsVisible = !m_windowsVisible;
}
