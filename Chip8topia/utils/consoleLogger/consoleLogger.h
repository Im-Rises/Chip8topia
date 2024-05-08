#pragma once

#if defined(BUILD_RELEASE)
#define LOG_ERROR(message) (void)0
#define LOG_WARNING(message) (void)0
#define LOG_INFO(message) (void)0
#define LOG_SUCCESS(message) (void)0
#else
#if defined(__EMSCRIPTEN__)
#include <iostream>
#define LOG_ERROR(message) std::cerr << message << '\n'
#define LOG_WARNING(message) std::cout << message << '\n'
#define LOG_INFO(message) std::cout << message << '\n'
#define LOG_SUCCESS(message) std::cout << message << '\n'
#else
#include <spdlog/spdlog.h>
#define LOG_ERROR(message) spdlog::error(message)
#define LOG_WARNING(message) spdlog::warn(message)
#define LOG_INFO(message) spdlog::info(message)
#define LOG_SUCCESS(message) spdlog::info(message)
#endif
#endif

// TODO: maybe use std::clog instead of std::cout for LOG_INFO but it's not correct in emscripten it correspond to an error message
