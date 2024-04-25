#include "Chip8topiaRomLoaderUi.h"

#include <IconsFontAwesome6.h>
#include <ImGuiNotify.hpp>
#include <imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten_browser_file.h>
#endif
#include <consoleLogger/consoleLogger.h>

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
            emscripten_browser_file::upload(Chip8RomLoader::CHIP8_ROM_FILE_EXTENSIONS_STRING, handle_upload_file, &chip8topia.getChip8Emulator());
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
    const auto windowSize = chip8topia.getCurrentDimensions();
    const ImVec2 windowDimensions(static_cast<float>(windowSize.first / 2), static_cast<float>(windowSize.second / 2));

    if (ImGuiFileDialog::Instance()->Display(FILE_DIALOG_NAME, ImGuiWindowFlags_NoCollapse, windowDimensions))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            try
            {
                const std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::vector<uint8> rom = Chip8RomLoader::loadRomFromPath(filePathName);
                chip8topia.getChip8Emulator().loadRom(rom);
                chip8topia.getChip8Emulator().setRomName(Chip8RomLoader::getRomNameFromPath(filePathName));
                ImGui::InsertNotification({ ImGuiToastType::Success, TOAST_DURATION_SUCCESS, "Rom loaded successfully" });
                LOG_INFO("Rom loaded successfully");
            }
            catch (const std::exception& e)
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, e.what() });
                LOG_ERROR(e.what());
            }
            catch (const char* const e)
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, e });
                LOG_ERROR(e);
            }
            catch (...)
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, "An unknown error occurred while loading the rom" });
                LOG_ERROR("An unknown error occurred while loading the rom");
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
        ImGui::InsertNotification({ ImGuiToastType::Success, TOAST_DURATION_SUCCESS, "Rom loaded successfully" });
        LOG_INFO("Rom loaded successfully");
    }
    catch (const std::exception& e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, e.what() });
        LOG_ERROR(e.what());
    }
    catch (const char* const e)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, e });
        LOG_ERROR(e);
    }
    catch (...)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, TOAST_DURATION_ERROR, "An unknown error occurred while loading the rom" });
        LOG_ERROR("An unknown error occurred while loading the rom");
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
#if defined(__EMSCRIPTEN__)
    config.path = DEFAULT_FOLDER_PATH;
#endif
    ImGuiFileDialog::Instance()->OpenDialog(FILE_DIALOG_NAME, "Select a game rom", Chip8RomLoader::CHIP8_ROM_FILE_EXTENSIONS_STRING, config);
}