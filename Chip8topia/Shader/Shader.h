#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <vector>
#include <string>

class Shader {
public:
    // #if defined(__EMSCRIPTEN__)
    //     Shader(const char* vertexCode, const char* fragmentCode);
    // #else
    Shader(const char* vertexPath, const char* fragmentPath);
    // #endif
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    Shader(Shader&&) = delete;
    auto operator=(Shader&&) -> Shader& = delete;
    ~Shader();

public:
    void compileFromFiles(const char* vertexPath, const char* fragmentPath);
    void compile(const char* vertexSource, const char* fragmentSource);
    static void checkCompileErrors(unsigned int shader, const std::string& type);

public:
    void use() const;
    [[maybe_unused]] [[nodiscard]] auto getID() const -> unsigned int;

public:
    [[maybe_unused]] void setBool(const std::string& name, bool value) const;
    [[maybe_unused]] void setInt(const std::string& name, int value) const;
    [[maybe_unused]] void setFloat(const std::string& name, float value) const;
    [[maybe_unused]] void setVec2(const std::string& name, float x, float y) const;
    [[maybe_unused]] void setVec3(const std::string& name, float x, float y, float z) const;
    [[maybe_unused]] void setVec4(const std::string& name, float x, float y, float z, float w) const;

private:
    GLuint m_ID;
};
