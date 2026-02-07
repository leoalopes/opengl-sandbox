#pragma once

#include "core/light/light.hpp"
#include "core/objects/point_primitive.hpp"
#include <memory>

class PointLight : public Light {
  public:
    PointPrimitive debugRenderer;
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular,
               float constant, float linear, float quadratic,
               std::shared_ptr<Shader> debugShader);

    void updateShader(std::string prefix, Shader *shader) override;

    void debugDraw();
};
