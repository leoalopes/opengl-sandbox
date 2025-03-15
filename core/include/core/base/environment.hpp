#pragma once
#include <glad/glad.h>

#include "core/graphics/shader.hpp"

#include <glm/glm.hpp>
#include <string>

class Environment {
  public:
    Environment(std::string basePath);

    void draw(glm::mat4 view, glm::mat4 projection);

  private:
    std::string basePath;

    unsigned int VAO, VBO;
    unsigned int textureId;

    Shader shader;

    void setupBuffer();
    void setupTexture();
    void setupTextureFace(GLenum face, std::string facePath);
};
