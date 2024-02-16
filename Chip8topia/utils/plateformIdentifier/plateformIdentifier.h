#pragma once

/* Platform: */

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define PLATFORM_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define PLATFORM_APPLE
#endif

#if defined(__linux__) || defined(__linux)
#define PLATFORM_LINUX
#endif

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
#define PLATFORM_UNIX
#endif

// #ifdef __EMSCRIPTEN__
// #define PLATFORM_WEB
// #endif


/* Build Mode: */

// #if defined(NDEBUG) || !defined(_DEBUG)
// #define BUILD_CONFIGURATION "Release"
// #define BUILD_RELEASE
// #else
// #define BUILD_CONFIGURATION "Debug"
// #define BUILD_DEBUG
// #endif

#if defined(NDEBUG) || !defined(_DEBUG)
#define BUILD_CONFIGURATION "Release"
#define BUILD_RELEASE
#else
#define BUILD_CONFIGURATION "Debug"
#define BUILD_DEBUG
#endif


/* Language: */

// #if defined(__cplusplus)
// #define LANGUAGE_CPP
// #else
// #define LANGUAGE_C
// #endif

// #ifdef __cplusplus
// #define EXTERN_C extern "C"
// #else
// #define EXTERN_C
// #endif

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