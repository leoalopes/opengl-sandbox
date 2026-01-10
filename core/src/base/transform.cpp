#include "core/base/transform.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::getMatrix(glm::vec3 positionOffset) {
    glm::mat4 matrix{1.0f};
    matrix = glm::translate(matrix, this->position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    matrix = glm::scale(matrix, this->scale);
    matrix = glm::translate(matrix, positionOffset);
    return matrix;
}
