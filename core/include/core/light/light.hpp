#pragma once

#include "core/graphics/shader.hpp"
#include "core/objects/point_primitive.hpp"

#include <glm/glm.hpp>
#include <memory>

class Light {
  public:
    PointPrimitive debugRenderer;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(std::shared_ptr<Shader> debugShader, glm::vec3 diffuse,
          glm::vec3 specular)
        : debugRenderer(debugShader), diffuse(diffuse), specular(specular) {}

    virtual void updateShader(std::string prefix, Shader *shader) {
        shader->use();
        shader->setVector(prefix + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
        shader->setVector(prefix + ".specular", specular.x, specular.y,
                          specular.z);
    }

    virtual void debugDraw() = 0;
};
