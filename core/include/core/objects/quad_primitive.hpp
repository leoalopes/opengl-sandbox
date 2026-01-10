#pragma once

#include "core/objects/object.hpp"
#include <glm/glm.hpp>

class QuadPrimitive : public Object {
  public:
    std::shared_ptr<Texture2D> texture;

    QuadPrimitive(std::shared_ptr<Shader> shader,
                  std::shared_ptr<Texture2D> texture);
};
