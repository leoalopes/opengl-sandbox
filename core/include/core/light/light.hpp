#pragma once

#include "core/graphics/shader.hpp"

#include <glm/glm.hpp>

class Light {
  public:
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 diffuse, glm::vec3 specular)
        : diffuse(diffuse), specular(specular) {}

    virtual void updateShader(std::string prefix, Shader *shader) {
        shader->use();
        shader->setVector(prefix + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
        shader->setVector(prefix + ".specular", specular.x, specular.y,
                          specular.z);
    }
};
