#pragma once

#include "glm/glm.hpp"

class Transform {
  public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();

    glm::mat4 getMatrix(glm::vec3 positionOffset = glm::vec3(0.0f));
};
