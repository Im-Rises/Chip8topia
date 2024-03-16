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

    [[nodiscard]] auto getBackgroundColor() -> ImVec4& { return m_backgroundColor; }
    [[nodiscard]] auto getMainPlaneColor() -> ImVec4& { return m_mainPlaneColor; }
    [[nodiscard]] auto getSecondaryPlaneColor() -> ImVec4& { return m_secondaryPlaneColor; }
    [[nodiscard]] auto getPixelsCommonColor() -> ImVec4& { return m_pixelsCommonColor; }

private:
    ShaderBW m_shaderLores;
    ShaderBW m_shaderHires;

    ShaderXoChip m_shaderXoChipLores;
    ShaderXoChip m_shaderXoChipHires;

    ImVec4 m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    ImVec4 m_mainPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    ImVec4 m_secondaryPlaneColor = { 0.8F, 0.8F, 0.8F, 1.0F };
    ImVec4 m_pixelsCommonColor = { 0.8F, 0.8F, 0.8F, 1.0F };
};
