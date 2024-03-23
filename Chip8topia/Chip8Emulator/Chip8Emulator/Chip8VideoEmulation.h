#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <memory>
#include <imgui.h>

#include "Chip8VideoEmulation/ShaderXoChip.h"

enum class EmulationColorMode
{
    Grayscale,
    Color
};

class Chip8CoreBase;
class Chip8VideoEmulation
{
private:
    static constexpr int SCREEN_BACKGROUND_COLOR_INDEX = 0;
    static constexpr int SCREEN_MAIN_PLANE_COLOR_INDEX = 15;

public:
    Chip8VideoEmulation();
    Chip8VideoEmulation(const Chip8VideoEmulation&) = delete;
    Chip8VideoEmulation(Chip8VideoEmulation&&) = delete;
    auto operator=(const Chip8VideoEmulation&) -> Chip8VideoEmulation& = delete;
    auto operator=(Chip8VideoEmulation&&) -> Chip8VideoEmulation& = delete;
    ~Chip8VideoEmulation() = default;

public:
    void reset();
    void resetToGrayscaleColors();
    void resetToColorColors();
    void updateTexture(const std::unique_ptr<Chip8CoreBase>& core);
    void update(const std::unique_ptr<Chip8CoreBase>& core, const float screenWidth, const float screenHeight, const float chip8AspectRatio);

    auto getColorMode() const -> EmulationColorMode;
    auto getColor(const int index) -> ImVec4&;
    auto getHiresPlaneTexture(const int planeIndex) -> GLuint;
    auto getLoresPlaneTexture(const int planeIndex) -> GLuint;
    //    auto getPlaneTexture(const PpuBase::PpuMode ppuMode, const int planeIndex) -> GLuint;

private:
    ShaderXoChip m_shaderXoChipLores;
    ShaderXoChip m_shaderXoChipHires;
    std::array<ImVec4, PpuBase::COLOR_COUNT> m_colors;
    EmulationColorMode m_colorMode = EmulationColorMode::Grayscale;

    //    ImVec4 m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    //    ImVec4 m_mainPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    //    ImVec4 m_subPlaneColor = { 0.6F, 0.6F, 0.6F, 1.0F };
    //    ImVec4 m_pixelsCommonColor = { 0.0F, 0.0F, 0.0F, 1.0F };

    //    ImVec4 m_backgroundColor = { 0.0F, 0.0F, 0.0F, 1.0F };
    //    ImVec4 m_mainPlaneColor = { 1.0F, 1.0F, 1.0F, 1.0F };
    //    ImVec4 m_subPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    //    ImVec4 m_pixelsCommonColor = { 0.6F, 0.6F, 0.6F, 1.0F };
};
