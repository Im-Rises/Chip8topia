#include "Chip8topiaRomLoaderUi.h"

#include <IconsFontAwesome6.h>
#include <ImGuiNotify.hpp>
#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten_browser_file.h>
#endif

#include "../Chip8Emulator/Chip8Emulator/Chip8RomLoader.h"
#include "../../Chip8topia.h"

Chip8topiaRomLoaderUi::Chip8topiaRomLoaderUi()
{
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.subscribe(this, &Chip8topiaRomLoaderUi::openRomWindow);
}

Chip8topiaRomLoaderUi::~Chip8topiaRomLoaderUi()
{
    Chip8topiaInputHandler::getInstance().m_OpenRomExplorerEvent.unsubscribe(this, &Chip8topiaRomLoaderUi::openRomWindow);
}

void Chip8topiaRomLoaderUi::drawFileMenu(Chip8topia& chip8topia)
{
    if (ImGui::BeginMenu(ICON_FA_FILE " File"))
    {
#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open rom...", "O"))
#else
        if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open rom...", "O"))
#endif
        {
            openRomWindow();
        }

#if defined(__EMSCRIPTEN__)
        if (ImGui::MenuItem(ICON_FA_UPLOAD " Upload rom...", "U"))
        {
            emscripten_browser_file::upload(Chip8RomLoader::CHIP8_ROM_FILE_EXTENSION, handle_upload_file, &chip8topia.getChip8Emulator());
        }
#endif

        if (ImGui::MenuItem(ICON_FA_EJECT " Eject rom", "E"))
        {
            chip8topia.getChip8Emulator().stop();
        }

#ifndef __EMSCRIPTEN__
        if (ImGui::MenuItem(ICON_FA_XMARK " Exit", "ESCAPE"))
        {
            chip8topia.closeRequest();
        }
#endif

        ImGui::EndMenu();
    }
}

void Chip8topiaRomLoaderUi::drawRomWindow(Chip8topia& chip8topia)
{
    std::pair windowSize = chip8topia.getWindowDimensions();

#ifdef __EMSCRIPTEN__
    const ImVec2 windowDimensions(static_cast<float>(windowSize.first / 2), static_cast<float>(windowSize.second / 2));
#else
    const ImVec2 windowDimensions(static_cast<float>(windowSize.first), static_cast<float>(windowSize.second));
#endif

    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME, ImGuiWindowFlags_NoCollapse, windowDimensions))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

            try
            {
                std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(filePathName);
                chip8topia.getChip8Emulator().loadRom(rom);
                chip8topia.getChip8Emulator().setRomName(Chip8RomLoader::getRomNameFromPath(filePathName));
                ImGui::InsertNotification({ ImGuiToastType::Success, 1000, "Rom loaded successfully" });
            }
            catch (const std::exception& e)
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, 3000, e.what() });
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

#if defined(__EMSCRIPTEN__)
void Chip8topiaRomLoaderUi::handle_upload_file(std::string const& filename, std::string const& mime_type, std::string_view buffer, void* chip8emulator)
{
#if !defined(BUILD_RELEASE)
    std::cout << "File uploaded: " << filename << " (" << mime_type << ")" << '\n';
#endif

    Chip8Emulator* chip8Emulator = static_cast<Chip8Emulator*>(chip8emulator);

    try
    {
        std::vector<uint8> rom = Chip8RomLoader::loadRomFromData(buffer);
        chip8Emulator->loadRom(rom);
        ImGui::InsertNotification({ ImGuiToastType::Success, 1000, "Rom loaded successfully" });
    }
    catch (const std::exception& e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, e.what() });
    }
}
#endif

void Chip8topiaRomLoaderUi::closeAllWindows()
{
    ImGuiFileDialog::Instance()->Close();
}

void Chip8topiaRomLoaderUi::openRomWindow()
{
    IGFD::FileDialogConfig config;
    config.path = DEFAULT_FOLDER_PATH;
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", Chip8RomLoader::CHIP8_ROM_FILE_EXTENSIONS_STRING, config);
}