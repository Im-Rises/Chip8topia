#pragma once

#include <PerformanceMonitor/PerformanceMonitor.h>

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
    void drawWindow();

private:
    PerformanceMonitor m_performanceMonitor;
};