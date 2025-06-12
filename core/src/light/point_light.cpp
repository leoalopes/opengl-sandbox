#include "core/light/point_light.hpp"

#include "core/base/common.hpp"
#include "core/interface/window.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight(glm::vec3 position, glm::vec3 diffuse,
                       glm::vec3 specular, float constant, float linear,
                       float quadratic)
    : Light(diffuse, specular), position(position), constant(constant),
      linear(linear), quadratic(quadratic) {}

void PointLight::updateShader(std::string prefix, Shader *shader) {
    Light::updateShader(prefix, shader);
    shader->setVector(prefix + ".position", position.x, position.y, position.z);

    shader->setFloat(prefix + ".constant", constant);
    shader->setFloat(prefix + ".linear", linear);
    shader->setFloat(prefix + ".quadratic", quadratic);
}

void PointLight::drawDebugBillboard() {
    Window &windowInstance = Window::getInstance();
    if (windowInstance.scene) {
        glm::mat4 model{1.0f};
        model = glm::translate(model, this->position);

        glm::mat4 view = windowInstance.scene->camera.getLookAt();

        /*unlintTexturedShader.use();*/
        /*unlintTexturedShader.setMatrix("model", glm::value_ptr(model));*/
        /*unlintTexturedShader.setMatrix("view", glm::value_ptr(view));*/
        /*unlintTexturedShader.setMatrix(*/
        /*    "projection",*/
        /*    glm::value_ptr(windowInstance.scene->projectionMatrix));*/
        /*// lightbulbTexture.use(0, &unlintTexturedShader,
         * "material.baseColor");*/
        /*quadBuffer.draw();*/
    }
}
