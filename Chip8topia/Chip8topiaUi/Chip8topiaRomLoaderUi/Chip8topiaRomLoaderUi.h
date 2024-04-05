#pragma once

#include <string>

class Chip8topia;
class Chip8topiaRomLoaderUi
{
private:
    static constexpr const char* const FILE_DIALOG_NAME = "RomFileWindowDialog";
#if defined(__EMSCRIPTEN__)
    static constexpr const char* const DEFAULT_FOLDER_PATH = "Chip8Games";
#endif
    static constexpr int TOAST_DURATION_SUCCESS = 1000;
    static constexpr int TOAST_DURATION_ERROR = 3000;

public:
    Chip8topiaRomLoaderUi();
    Chip8topiaRomLoaderUi(const Chip8topiaRomLoaderUi&) = delete;
    Chip8topiaRomLoaderUi(Chip8topiaRomLoaderUi&&) = delete;
    auto operator=(const Chip8topiaRomLoaderUi&) -> Chip8topiaRomLoaderUi& = delete;
    auto operator=(Chip8topiaRomLoaderUi&&) -> Chip8topiaRomLoaderUi& = delete;
    ~Chip8topiaRomLoaderUi();

public:
    void drawFileMenu(Chip8topia& chip8topia);
    void drawRomWindow(Chip8topia& chip8topia);
    void closeAllWindows();

private:
#if defined(__EMSCRIPTEN__)
    static void handle_upload_file(std::string const& filename, std::string const& mime_type, std::string_view buffer, void* chip8emulator);
#endif
    void openRomWindow();
};
