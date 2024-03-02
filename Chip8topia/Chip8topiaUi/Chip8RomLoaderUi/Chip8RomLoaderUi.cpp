#include "Chip8RomLoaderUi.h"

#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#if !defined(BUILD_RELEASE)
#include <spdlog/spdlog.h>
#include <iostream>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten_browser_file.h>
#endif

#include "../Chip8Emulator/Chip8Emulator/Chip8RomLoader.h"
#include "../../Chip8topia.h"

Chip8RomLoaderUi::Chip8RomLoaderUi() {
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.subscribe(this, &Chip8RomLoaderUi::openRomWindow);
}

Chip8RomLoaderUi::~Chip8RomLoaderUi() {
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.unsubscribe(this, &Chip8RomLoaderUi::openRomWindow);
}

void Chip8RomLoaderUi::drawFileMenu(Chip8topia& chip8topia) {
    if (ImGui::BeginMenu("File"))
    {
#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem("Open integrated rom..."))
#else
        if (ImGui::MenuItem("Open rom...", "O"))
#endif
        {
            openRomWindow();
        }

#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem("Open rom"))
        {
            emscripten_browser_file::upload(Chip8RomLoader::CHIP8_ROM_FILE_EXTENSION, handle_upload_file, &chip8topia.getChip8Emulator());
        }
#endif

        if (ImGui::MenuItem("Eject rom", "E"))
        {
            chip8topia.getChip8Emulator().stop();
        }

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem("Exit", "ESCAPE"))
        {
            chip8topia.closeRequest();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8RomLoaderUi::drawRomWindow(Chip8topia& chip8topia) {
    std::pair windowSize = chip8topia.getWindowDimensions();

#ifdef __EMSCRIPTEN__
    const ImVec2 windowDimensions(static_cast<float>(windowSize.first / 2), static_cast<float>(windowSize.second / 2));
#else
    const ImVec2 windowDimensions(static_cast<float>(windowSize.first), static_cast<float>(windowSize.second));
#endif

    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME, ImGuiWindowFlags_NoCollapse, windowDimensions))
    //    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME, ImGuiWindowFlags_NoCollapse))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

            try
            {
                std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(filePathName);
                chip8topia.getChip8Emulator().loadRom(rom);
                chip8topia.getChip8Emulator().setRomName(Chip8RomLoader::getRomNameFromPath(filePathName));
            }
            catch (const std::exception& e)
            {
#if !defined(BUILD_RELEASE)
                spdlog::error(e.what());
#endif
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

#if defined(__EMSCRIPTEN__)
void Chip8RomLoaderUi::handle_upload_file(std::string const& filename, std::string const& mime_type, std::string_view buffer, void* chip8emulator) {
#if !defined(BUILD_RELEASE)
    std::cout << "File uploaded: " << filename << " (" << mime_type << ")" << '\n';
#endif

    Chip8Emulator* chip8Emulator = static_cast<Chip8Emulator*>(chip8emulator);

    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromData(buffer);
        chip8Emulator->loadRom(rom);
    }
    catch (const std::exception& e)
    {
#if !defined(BUILD_RELEASE)
        std::cerr << e.what() << '\n';
#endif
    }
}
#endif

void Chip8RomLoaderUi::closeAllWindows() {
    ImGuiFileDialog::Instance()->Close();
}

void Chip8RomLoaderUi::openRomWindow() {
    IGFD::FileDialogConfig config;
    config.path = DEFAULT_FOLDER_PATH;
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", Chip8RomLoader::CHIP8_ROM_FILE_EXTENSION, config);
}
