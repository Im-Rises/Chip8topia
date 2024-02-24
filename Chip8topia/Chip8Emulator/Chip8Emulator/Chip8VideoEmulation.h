#pragma once

#include <memory>

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include "Chip8VideoEmulation/ShaderBW.h"

// TODO: I think the best solution is to create a class that will have 2 different shaders, one for the 64x32 and another for the 128x64 resolution.
// Depending on the resolution, the class will use the correct shader.
// That means the Chip8Core in the Chip8Emulator will always be loaded, but the core will be changed when changing the console version.

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

private:
    ShaderBW m_shaderLores;
    ShaderBW m_shaderHires;
};
