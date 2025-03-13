#include <glad/glad.h>

#include "core/shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::string readFile(const char *path) {
    std::basic_ifstream<char> file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);

    std::basic_stringstream<char> fileStream;
    fileStream << file.rdbuf();

    file.close();
    return fileStream.str();
}

unsigned int compileShader(GLenum type, std::string sourceCode) {
    const char *code = sourceCode.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::"
                  << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                  << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    return shader;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode, fragmentCode;
    try {
        vertexCode = readFile(vertexPath);
        fragmentCode = readFile(fragmentPath);
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    this->programId = glCreateProgram();
    glAttachShader(this->programId, vertexShader);
    glAttachShader(this->programId, fragmentShader);
    glLinkProgram(this->programId);

    int success;
    glGetProgramiv(this->programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->programId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(this->programId); }

bool Shader::hasUniform(const std::string &name) {
    return glGetUniformLocation(this->programId, name.c_str()) != -1;
}

void Shader::setInt(const std::string &name, int value) {
    if (this->hasUniform(name)) {
        glUniform1i(glGetUniformLocation(this->programId, name.c_str()), value);
    }
}

void Shader::setFloat(const std::string &name, float value) {
    if (this->hasUniform(name)) {
        glUniform1f(glGetUniformLocation(this->programId, name.c_str()), value);
    }
}

void Shader::setVector(const std::string &name, float x, float y, float z) {
    if (this->hasUniform(name)) {
        glUniform3f(glGetUniformLocation(this->programId, name.c_str()), x, y,
                    z);
    }
}

void Shader::setMatrix(const std::string &name, float *value) {
    if (this->hasUniform(name)) {
        glUniformMatrix4fv(glGetUniformLocation(this->programId, name.c_str()),
                           1, GL_FALSE, value);
    }
}
