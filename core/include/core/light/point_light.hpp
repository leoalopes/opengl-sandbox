#pragma once

#include "core/light/light.hpp"

class PointLight : public Light {
  public:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular,
               float constant, float linear, float quadratic);

    void updateShader(std::string prefix, Shader *shader) override;

    void drawDebugBillboard();
};
