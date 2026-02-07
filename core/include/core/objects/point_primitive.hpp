#pragma once

#include "glm/glm.hpp"

#include "core/graphics/shader.hpp"
#include "core/objects/point_buffer.hpp"

#include <memory>

class PointPrimitive {
  public:
    PointBuffer buffer;
    std::shared_ptr<Shader> shader;

    glm::vec3 color;
    float size = 50.0f;

    PointPrimitive(std::shared_ptr<Shader> shader);

    void draw(glm::vec3 position);
};
