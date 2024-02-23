#include "Chip8RomLoaderUi.h"

#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <iostream>

#include "../../Chip8topia.h"

Chip8RomLoaderUi::Chip8RomLoaderUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.subscribe(this, &Chip8RomLoaderUi::openRomWindow);
#endif
}

Chip8RomLoaderUi::~Chip8RomLoaderUi() {
#ifndef __EMSCRIPTEN__
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.unsubscribe(this, &Chip8RomLoaderUi::openRomWindow);
#endif
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
            emscripten_browser_file::upload(CHIP8_ROM_FILE_EXTENSION, Chip8topia::handle_upload_file, &chip8topia.getChip8Emulator());
        }
#endif

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

    // TODO: Maybe Add imgui.ini to the .data emscripten build
    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME, ImGuiWindowFlags_NoCollapse, ImVec2(static_cast<float>(windowSize.first), static_cast<float>(windowSize.second))))
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
                std::cerr << e.what() << '\n';
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

#if defined(__EMSCRIPTEN__)
void Chip8topiaUi::handle_upload_file(std::string const& filename, std::string const& mime_type, std::string_view buffer, void* chip8emulator) {
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

void Chip8RomLoaderUi::openRomWindow() {
    IGFD::FileDialogConfig config;
    config.path = DEFAULT_FOLDER_PATH;
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", Chip8RomLoader::CHIP8_ROM_FILE_EXTENSION, config);
}
