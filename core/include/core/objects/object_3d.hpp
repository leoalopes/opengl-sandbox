#pragma once

#include "core/base/transform.hpp"

#include <memory>
#include <vector>

class Object3D {
  public:
    Transform transform;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Object3D> parent;
    std::vector<std::shared_ptr<Object3D>> children;
};
