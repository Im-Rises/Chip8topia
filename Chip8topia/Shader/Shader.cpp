#include "Shader.h"

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>


Shader::Shader(const char* vertexPath, const char* fragmentPath) : m_ID(0) {
    compileFromFiles(vertexPath, fragmentPath);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

void Shader::compileFromFiles(const char* vertexPath, const char* fragmentPath) {
#ifndef __EMSCRIPTEN__
    // Retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure stream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string and create shader
        std::string const vertexCodeStr = vShaderStream.str();
        const char* vertexCode = vertexCodeStr.c_str();

        std::string const fragmentCodeStr = fShaderStream.str();
        const char* fragmentCode = fragmentCodeStr.c_str();

        compile(vertexCode, fragmentCode);
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
    }

#else // TODO: Modify put this shader code out of the general Shader class!!!

    static constexpr char* vertexShaderSource =
        R"(#version 300 es

            layout(location = 0) in vec3 a_texCoord;

            out vec2 v_texCoord;

            void main()
            {
            v_texCoord = vec2(a_texCoord.x * 0.5 + 0.5, 1.0 - (a_texCoord.y * 0.5 + 0.5));
            gl_Position = vec4(a_texCoord.x, a_texCoord.y, a_texCoord.z, 1.0);
            }
    )";

    static constexpr char* fragmentShaderSource =
        R"(#version 300 es

            precision mediump float;

            in vec2 v_texCoord;

            out vec4 o_fragColor;

            uniform sampler2D u_ourTexture;
            uniform vec4 u_backgroundColor;
            uniform vec4 u_foregroundColor;

            void main()
            {
                vec4 color = texture(u_ourTexture, v_texCoord);
                if (color.r > 0.0)
                {
                    o_fragColor = u_foregroundColor;
                }
                else
                {
                    o_fragColor = u_backgroundColor;
                }
            }
    )";

    compile(vertexShaderSource, fragmentShaderSource);

#endif
}

void Shader::compile(const char* vertexSource, const char* fragmentSource) {
    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success = 0;
    std::array<char, 1024> infoLog{};
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog.data()
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog.data()
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::use() const {
    glUseProgram(m_ID);
}

[[maybe_unused]] auto Shader::getID() const -> unsigned int {
    return m_ID;
}

[[maybe_unused]] void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
}

[[maybe_unused]] void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}
