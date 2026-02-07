#include "core/light/point_light.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight(glm::vec3 position, glm::vec3 diffuse,
                       glm::vec3 specular, float constant, float linear,
                       float quadratic, std::shared_ptr<Shader> debugShader)
    : Light(debugShader, diffuse, specular), position(position),
      constant(constant), linear(linear), quadratic(quadratic) {}

void PointLight::updateShader(std::string prefix, Shader *shader) {
    Light::updateShader(prefix, shader);
    shader->setVector(prefix + ".position", position.x, position.y, position.z);

    shader->setFloat(prefix + ".constant", constant);
    shader->setFloat(prefix + ".linear", linear);
    shader->setFloat(prefix + ".quadratic", quadratic);
}

void PointLight::debugDraw() {
    this->debugRenderer.color = this->diffuse * 2.0f;
    this->debugRenderer.draw(this->position);
}
