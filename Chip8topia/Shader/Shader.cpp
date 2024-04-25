#include "Shader.h"

#include <fstream>
#include <sstream>
#include <array>
#include <fmt/format.h>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) : m_ID(0)
{
    compileFromFiles(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::compileFromFiles(const char* vertexPath, const char* fragmentPath)
{
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
        Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger(fmt::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ {}", e.what()), nullptr);
    }
    catch (const char* const e)
    {
        Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger(fmt::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ {}", e), nullptr);
    }
    catch (...)
    {
        Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ Unknown error", nullptr);
    }
}

void Shader::compile(const char* vertexSource, const char* fragmentSource)
{
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

void Shader::checkCompileErrors(unsigned int shader, const std::string& type)
{
    int success = 0;
    std::array<char, 1024> infoLog{};
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
            Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger(fmt::format("ERROR::SHADER_COMPILATION_ERROR of type: {} \n {}", type, infoLog.data()), nullptr);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
            Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger(fmt::format("ERROR::PROGRAM_LINKING_ERROR of type: {} \n {}", type, infoLog.data()), nullptr);
        }
    }
}

void Shader::use() const
{
    glUseProgram(m_ID);
}

[[maybe_unused]] auto Shader::getID() const -> unsigned int
{
    return m_ID;
}

[[maybe_unused]] void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
}

[[maybe_unused]] void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void Shader::setVec4Array(const std::string& name, const std::vector<Vec4>& vec4Array) const
{
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), static_cast<GLsizei>(vec4Array.size()), reinterpret_cast<const float*>(vec4Array.data()));
}
