#pragma once

#include <vector>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    Shader(Shader&&) = delete;
    auto operator=(Shader&&) -> Shader& = delete;
    virtual ~Shader();

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
    unsigned int ID;
};
