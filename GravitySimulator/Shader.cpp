#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertPath, const char* fragPath) {
    // vertex shaders!
    //-----------------------------------------------------------------
    std::string vertexCode = readFile(vertPath);
    const char* vShaderCode = vertexCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");
    
    // fragment shaders!
    //-----------------------------------------------------------------
    std::string fragmentCode = readFile(fragPath);
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // shader program linking!
    //-----------------------------------------------------------------
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(ID);
}

// utility
//-------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID,name.c_str()), value);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID,name.c_str()), 1, &value[0]);
}
void Shader::setMat4(const std::string name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

// private
//-------------------------------------------------------------------
std::string Shader::readFile(const char* filePath) {
    std::ifstream file;
    std::stringstream buffer;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filePath);
        buffer << file.rdbuf();
        file.close();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR: Failed to read shader file: " << filePath << std::endl;
    }

    return buffer.str();
}
void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int  success;
    char infoLog[1024];

    if (type == "VERTEX" || type == "FRAGMENT") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::"<< type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    } else if (type == "program") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
}