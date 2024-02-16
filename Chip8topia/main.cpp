// // Doesn't work with MSVC
// auto getCPppVersion() -> const char* {
//    switch (__cplusplus)
//    {
//    case 202101L:
//        return "C++23";
//    case 202002L:
//        return "C++20";
//    case 201703L:
//        return "C++17";
//    case 201402L:
//        return "C++14";
//    case 201103L:
//        return "C++11";
//    case 199711L:
//        return "C++98";
//    default:
//        return (&"pre-standard C++. "[__cplusplus]);
//    }
//}

#if defined(_WIN32) && !defined(_DEBUG)
#define WIN32_RELEASE 1
#endif

#if defined(WIN32_RELEASE)
#include <Windows.h>
#else
#include <iostream>
#endif

#include "Chip8topia.h"

#if defined(WIN32_RELEASE)
auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) -> int {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
#else
auto main(int argc, char* argv[]) -> int {
    (void)argc;
    (void)argv;
#endif

    Chip8topia chip8topia;

#ifndef WIN32_RELEASE
    std::cout << Chip8topia::PROJECT_NAME << " " << Chip8topia::PROJECT_VERSION << std::endl
              << Chip8topia::PROJECT_LINK << std::endl
              << Chip8topia::PROJECT_AUTHOR << "\n"
              << std::endl
              << std::endl;

    std::cout << "Build infos:" << std::endl
              << " - C++ standard: " << __cplusplus << std::endl
    //              << "Compiler: " << __VERSION__ << std::endl
#if defined(_DEBUG)
              << " - Build type: Release" << std::endl // Not display because no console in release
#else
              << " - Build type: Debug" << std::endl
#endif
              << " - Build date: " << __DATE__ << " " << __TIME__ << std::endl
              << std::endl;
#endif

    return chip8topia.run();
}
