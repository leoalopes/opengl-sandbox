#pragma once

#include "core/light/light.hpp"

class DirectionalLight : public Light {
  public:
    glm::vec3 ambient;
    glm::vec3 direction;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                     glm::vec3 specular);

    void updateShader(std::string prefix, Shader *shader) override;
};
