#pragma once

#include <string>

class Shader {
    unsigned int programId;

  public:
    Shader(const char *vertexPath, const char *fragmentPath);

    void use();

    bool hasUniform(const std::string &name);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVector(const std::string &name, float x, float y, float z);
    void setMatrix(const std::string &name, float *value);
};
