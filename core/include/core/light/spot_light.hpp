#pragma once

#include "core/light/light.hpp"

class SpotLight : public Light {
  public:
    glm::vec3 position;
    glm::vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff,
              float outerCutOff, glm::vec3 diffuse, glm::vec3 specular,
              float constant, float linear, float quadratic);

    void updateShader(std::string prefix, Shader *shader) override;
};
