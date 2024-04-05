#pragma once

#include <PerformanceMonitor/PerformanceMonitor.h>

class Chip8topia;
class Chip8topiaPerformanceMonitor
{
public:
    Chip8topiaPerformanceMonitor() = default;
    Chip8topiaPerformanceMonitor(const Chip8topiaPerformanceMonitor&) = delete;
    auto operator=(const Chip8topiaPerformanceMonitor&) -> Chip8topiaPerformanceMonitor& = delete;
    Chip8topiaPerformanceMonitor(Chip8topiaPerformanceMonitor&&) = delete;
    auto operator=(Chip8topiaPerformanceMonitor&&) -> Chip8topiaPerformanceMonitor& = delete;
    ~Chip8topiaPerformanceMonitor() = default;

public:
    void drawWindow(Chip8topia& chip8topia, bool isMainBarOpen);

private:
#if !defined(__EMSCRIPTEN__)
    PerformanceMonitor m_performanceMonitor;
#endif
};
