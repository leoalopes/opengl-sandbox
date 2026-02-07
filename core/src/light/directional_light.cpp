#include "core/light/directional_light.hpp"
#include "core/interface/window.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient,
                                   glm::vec3 diffuse, glm::vec3 specular,
                                   std::shared_ptr<Shader> debugShader)
    : Light(debugShader, diffuse, specular), ambient(ambient),
      direction(direction) {
    debugRenderer.size = 300.0f;
}

void DirectionalLight::updateShader(std::string prefix, Shader *shader) {
    Light::updateShader(prefix, shader);
    shader->setVector(prefix + ".ambient", ambient.x, ambient.y, ambient.z);
    shader->setVector(prefix + ".direction", direction.x, direction.y,
                      direction.z);
}

void DirectionalLight::debugDraw() {
    this->debugRenderer.color = this->diffuse * 2.0f;

    Window &windowInstance = Window::getInstance();
    glm::vec3 position = windowInstance.scene->camera.location;
    position -= this->direction * 10.0f;
    this->debugRenderer.draw(position);
}
