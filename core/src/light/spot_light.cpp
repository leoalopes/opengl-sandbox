#include "core/light/spot_light.hpp"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff,
                     float outerCutOff, glm::vec3 diffuse, glm::vec3 specular,
                     float constant, float linear, float quadratic)
    : Light(diffuse, specular), position(position), direction(direction),
      cutOff(cutOff), outerCutOff(outerCutOff), constant(constant),
      linear(linear), quadratic(quadratic) {}

void SpotLight::updateShader(std::string prefix, Shader *shader) {
    Light::updateShader(prefix, shader);
    shader->setVector(prefix + ".position", position.x, position.y, position.z);
    shader->setVector(prefix + ".direction", direction.x, direction.y,
                      direction.z);

    shader->setFloat(prefix + ".cutOff", cutOff);
    shader->setFloat(prefix + ".outerCutOff", outerCutOff);

    shader->setFloat(prefix + ".constant", constant);
    shader->setFloat(prefix + ".linear", linear);
    shader->setFloat(prefix + ".quadratic", quadratic);
}
