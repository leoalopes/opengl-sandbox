#pragma once

#include "core/objects/object.hpp"
#include <glm/glm.hpp>

class SpherePrimitive : public Object {
  public:
    std::shared_ptr<Texture2D> texture;

    SpherePrimitive(std::shared_ptr<Shader> shader,
                    std::shared_ptr<Texture2D> texture,
                    int latitudeBands = 32,
                    int longitudeBands = 32);
};
