#pragma once

#if defined(BUILD_RELEASE)
#define LOG_ERROR(message)
#define LOG_WARNING(message)
#define LOG_INFO(message)
#define LOG_SUCCESS(message)
#else
#if defined(__EMSCRIPTEN__)
#include <iostream>
#define LOG_ERROR(message) std::cerr << message << '\n'
#define LOG_WARNING(message) std::cout << message << '\n'
#define LOG_INFO(message) std::clog << message << '\n'
#define LOG_SUCCESS(message) std::cout << message << '\n'
#else
#include <spdlog/spdlog.h>
#define LOG_ERROR(message) spdlog::error(message)
#define LOG_WARNING(message) spdlog::warn(message)
#define LOG_INFO(message) spdlog::info(message)
#define LOG_SUCCESS(message) spdlog::info(message)
#endif
#endif