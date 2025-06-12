#pragma once

#include "core/base/transform.hpp"
#include "core/objects/object_buffer.hpp"

#include <memory>
#include <vector>

class Object3D {
  public:
    Transform transform;
    std::shared_ptr<ObjectBuffer> buffer;

    std::shared_ptr<Object3D> parent;
    std::vector<std::shared_ptr<Object3D>> children;
};
