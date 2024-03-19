#if defined(PLATFORM_WINDOWS) && defined(BUILD_RELEASE)
#define WINRELEASE
#endif

#if defined(WINRELEASE)
#include <Windows.h>
#else
#include <iostream>
#endif

#include "Chip8topia.h"

#if defined(WINRELEASE)
auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) -> int
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
#else // Windows Debug, Linux, Apple
auto main(int argc, char* argv[]) -> int
{
    (void)argc;
    (void)argv;
#endif

    Chip8topia chip8topia;

#if !(defined(WINRELEASE))
    std::cout << Chip8topia::PROJECT_NAME << " " << Chip8topia::PROJECT_VERSION << '\n'
              << Chip8topia::PROJECT_LINK << '\n'
              << Chip8topia::PROJECT_AUTHOR << "\n"
              << '\n'
              << '\n';

    std::cout << "Build infos:" << '\n'
              << " - Platform: " << PLATFORM_NAME << '\n'
              << " - C++ version: " << PROJECT_CPP_VERSION << '\n'
              //              << " - Build type: " << BUILD_CONFIG_STRING << '\n'
              << " - Build date: " << __DATE__ << " " << __TIME__ << '\n'
              << '\n';
#endif

    return chip8topia.run();
}
