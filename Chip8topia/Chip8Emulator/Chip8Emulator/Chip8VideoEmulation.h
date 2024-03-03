#pragma once

#include <memory>

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include "Chip8VideoEmulation/ShaderBW.h"

class Chip8CoreBase;
class Chip8VideoEmulation {
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
    void update(const std::unique_ptr<Chip8CoreBase>& core);

    [[nodiscard]] auto getBackgroundColor() -> ImVec4& { return m_backgroundColor; }
    [[nodiscard]] auto getForegroundColor() -> ImVec4& { return m_foregroundColor; }

private:
    ShaderBW m_shaderLores;
    ShaderBW m_shaderHires;

    ImVec4 m_backgroundColor = { 0.3F, 0.3F, 0.3F, 1.0F };
    ImVec4 m_foregroundColor = { 0.8F, 0.8F, 0.8F, 1.0F };
};
