#include <iostream>

#include "Chip8topia.h"

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

auto main(int argc, char* argv[]) -> int {
    (void)argc;
    (void)argv;

    Chip8topia chip8topia;

    std::cout << Chip8topia::PROJECT_NAME << " " << Chip8topia::PROJECT_VERSION << std::endl
              << Chip8topia::PROJECT_LINK << std::endl
              << Chip8topia::PROJECT_AUTHOR << "\n"
              << std::endl
              << std::endl;

    std::cout << "System and dependencies infos:" << std::endl
              << " - OpenGL vendor " << Chip8topia::getOpenGLVendor() << std::endl
              << " - OpenGL version " << Chip8topia::getOpenGLVersion() << std::endl
              << " - OpenGL GLSL version " << Chip8topia::getGLSLVersion() << std::endl
              << " - GLFW version " << Chip8topia::getGLFWVersion() << std::endl
              << " - Glad version " << Chip8topia::getGladVersion() << std::endl
              << " - ImGui version " << Chip8topia::getImGuiVersion() << std::endl
              << std::endl;

    std::cout << "Build infos:" << std::endl
              << " - C++ standard: " << __cplusplus << std::endl
    //              << "Compiler: " << __VERSION__ << std::endl
#if defined(NDEBUG)
              << " - Build type: Release" << std::endl
#else
              << " - Build type: Debug" << std::endl
#endif
              << " - Build date: " << __DATE__ << " " << __TIME__ << std::endl
              << std::endl;

    return chip8topia.run();
}
