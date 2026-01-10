#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float DEFAULT_FOV = 45.0f;

const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);
const glm::vec3 DEFAULT_TRANSFORM = glm::vec3(0, 0, 0);
const glm::vec3 DEFAULT_FORWARD_VECTOR = glm::vec3(0, 0, -1);

class Camera {
  public:
    glm::vec3 location;
    glm::vec3 forwardVector;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 worldUp;

    float fov;
    float pitch;
    float yaw;

    void
    updateCameraVectorsFromForwardVector(glm::vec3 forwardVector,
                                         glm::vec3 worldUpVector = WORLD_UP);
    void updateCameraVectors();

    Camera(glm::vec3 transform = DEFAULT_TRANSFORM,
           glm::vec3 worldUp = WORLD_UP, float fov = DEFAULT_FOV);

    glm::mat4 getLookAt();

    float getFov() const { return this->fov; }
    void setFov(float fov) { this->fov = fov; }

    glm::vec3 getLocation() const { return this->location; }
    void setLocation(glm::vec3 location) { this->location = location; }

    glm::vec3 getForwardVector() const { return this->forwardVector; }
    glm::vec3 getRightVector() const { return this->rightVector; }
    glm::vec3 getUpVector() const { return this->upVector; }

    void updatePitchYaw(float yawOffset, float pitchOffset);
};
