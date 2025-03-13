#include "core/light/directional_light.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient,
                                   glm::vec3 diffuse, glm::vec3 specular)
    : Light(diffuse, specular), ambient(ambient), direction(direction) {}

void DirectionalLight::updateShader(std::string prefix, Shader *shader) {
    Light::updateShader(prefix, shader);
    shader->setVector(prefix + ".ambient", ambient.x, ambient.y, ambient.z);
    shader->setVector(prefix + ".direction", direction.x, direction.y,
                      direction.z);
}
