#include <glad/glad.h>

#include "core/base/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 location, glm::vec3 worldUp, float fov)
    : fov(fov), location(location), worldUp(worldUp) {
    this->updateCameraVectors();
    this->pitch = 0.0f;
    this->yaw = -90.0f;
}

void Camera::updateCameraVectors() {
    this->forwardVector = {cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                           sin(glm::radians(pitch)),
                           sin(glm::radians(yaw)) * cos(glm::radians(pitch))};
    this->forwardVector = glm::normalize(this->forwardVector);

    this->rightVector =
        glm::normalize(glm::cross(this->forwardVector, this->worldUp));
    this->upVector =
        glm::normalize(glm::cross(this->rightVector, this->forwardVector));
}

glm::mat4 Camera::getLookAt() {
    return glm::lookAt(this->location, this->location + this->forwardVector,
                       this->upVector);
}

void Camera::updatePitchYaw(float yawOffset, float pitchOffset) {
    yaw += yawOffset;

    pitch += pitchOffset;
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    this->updateCameraVectors();
}
