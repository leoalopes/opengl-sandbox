#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "core/interface/window.hpp"
#include "core/objects/point_primitive.hpp"

#include <memory>

PointPrimitive::PointPrimitive(std::shared_ptr<Shader> shader)
    : shader(shader) {}

void PointPrimitive::draw(glm::vec3 position) {
    Window &windowInstance = Window::getInstance();

    this->shader->use();
    this->shader->setFloat("size", this->size);
    this->shader->setVector("color", this->color.r, this->color.g,
                            this->color.b);

    glm::mat4 model{1.0f};
    model = glm::translate(model, position);
    glm::mat4 view = windowInstance.scene->camera.getLookAt();
    glm::mat4 projection =
        glm::perspective(glm::radians(windowInstance.scene->camera.fov),
                         (float)windowInstance.scene->screenWidth /
                             (float)windowInstance.scene->screenHeight,
                         0.1f, 100.0f);

    this->shader->setMatrix("model", glm::value_ptr(model));
    this->shader->setMatrix("view", glm::value_ptr(view));
    this->shader->setMatrix("projection", glm::value_ptr(projection));

    this->buffer.draw();
}
