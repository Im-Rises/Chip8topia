#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <memory>
#include <imgui.h>

#include "Chip8VideoEmulation/ShaderBW.h"
#include "Chip8VideoEmulation/ShaderXoChip.h"

class Chip8CoreBase;
class Chip8VideoEmulation
{
public:
    Chip8VideoEmulation();
    Chip8VideoEmulation(const Chip8VideoEmulation&) = delete;
    Chip8VideoEmulation(Chip8VideoEmulation&&) = delete;
    auto operator=(const Chip8VideoEmulation&) -> Chip8VideoEmulation& = delete;
    auto operator=(Chip8VideoEmulation&&) -> Chip8VideoEmulation& = delete;
    ~Chip8VideoEmulation() = default;

public:
    void reset();
    void updateTexture(const std::unique_ptr<Chip8CoreBase>& core);
    void update(const std::unique_ptr<Chip8CoreBase>& core, const float screenWidth, const float screenHeight, const float chip8AspectRatio);

    auto getColor(const int index) -> ImVec4& { return m_colors[index]; }
    //    auto getColors() -> std::array<ImVec4, 16>& { return m_colors; }

    //    [[nodiscard]] auto getBackgroundColor() -> ImVec4& { return m_backgroundColor; }
    //    [[nodiscard]] auto getMainPlaneColor() -> ImVec4& { return m_mainPlaneColor; }
    //    [[nodiscard]] auto getSubPlaneColor() -> ImVec4& { return m_subPlaneColor; }
    //    [[nodiscard]] auto getPixelsCommonColor() -> ImVec4& { return m_pixelsCommonColor; }

    void resetColors();

private:
    // TODO: No needs to use ShaderBW, we can use the XoChip shader for all cores and only update the first plane for Chip8, SCHip11 and SCHipC
    ShaderBW m_shaderLores;
    ShaderBW m_shaderHires;

    ShaderXoChip m_shaderXoChipLores;
    ShaderXoChip m_shaderXoChipHires;

    std::array<ImVec4, 16> m_colors;


    //    ImVec4 m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    //    ImVec4 m_mainPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    //    ImVec4 m_subPlaneColor = { 0.6F, 0.6F, 0.6F, 1.0F };
    //    ImVec4 m_pixelsCommonColor = { 0.0F, 0.0F, 0.0F, 1.0F };

    //    ImVec4 m_backgroundColor = { 0.0F, 0.0F, 0.0F, 1.0F };
    //    ImVec4 m_mainPlaneColor = { 1.0F, 1.0F, 1.0F, 1.0F };
    //    ImVec4 m_subPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    //    ImVec4 m_pixelsCommonColor = { 0.6F, 0.6F, 0.6F, 1.0F };
};
