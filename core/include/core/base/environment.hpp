#pragma once
#include <glad/glad.h>

#include "core/graphics/shader.hpp"
#include "core/graphics/texture_3d.hpp"

#include <glm/glm.hpp>
#include <string>

class Environment {
  public:
    Environment(std::array<std::string, 6> paths);

    void draw(glm::mat4 view, glm::mat4 projection);

  private:
    unsigned int VAO, VBO;
    Texture3D texture;

    Shader shader;

    void setupBuffer();
};
