#pragma once

#include <string>

class Chip8topia;
class Chip8topiaRomLoaderUi
{
private:
    static constexpr auto FILE_DIALOG_NAME = "RomFileWindowDialog";
#if defined(__EMSCRIPTEN__)
    static constexpr auto DEFAULT_FOLDER_PATH = "Chip8Games";
#else
    static constexpr auto DEFAULT_FOLDER_PATH = ".";
#endif

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
