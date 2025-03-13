#pragma once

#include "glm/glm.hpp"

class Transform {
  public:
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();

    glm::mat4 getMatrix();
};
