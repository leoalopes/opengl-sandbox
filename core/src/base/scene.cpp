#include "core/base/scene.hpp"
#include "core/base/camera.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <string>

Scene::Scene(unsigned int screenWidth, unsigned int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      directionalLight(DEFAULT_DL_DIRECTION, DEFAULT_DL_AMBIENT,
                       DEFAULT_DL_DIFFUSE, DEFAULT_DL_SPECULAR),
      flashlight(glm::vec3(0.0f), glm::vec3(0.0f),
                 glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(20.0f)),
                 glm::vec3(0.9f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f) {
    this->updateProjectionMatrix(&this->camera);
};

void Scene::updateProjectionMatrix(Camera *renderCamera) {
    this->projectionMatrix = glm::perspective(
        glm::radians(renderCamera->fov),
        (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

std::multimap<float, std::shared_ptr<Model>>
Scene::getModelsByDistance(Camera *renderCamera) {
    std::multimap<float, std::shared_ptr<Model>> modelsByDistance;
    for (const auto &model : this->models) {
        float distance =
            glm::length(model->transform.position - renderCamera->location);
        modelsByDistance.insert({distance, model});
    }

    return modelsByDistance;
}

void Scene::draw() { this->draw(&this->camera); }

void Scene::draw(Camera *renderCamera) {
    this->updateProjectionMatrix(renderCamera);

    glm::mat4 viewMatrix = renderCamera->getLookAt();
    flashlight.position = renderCamera->location;
    flashlight.direction = renderCamera->forwardVector;

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    if (this->environment) {
        this->environment->draw(viewMatrix, projectionMatrix);
    }

    auto modelsByDistance = this->getModelsByDistance(renderCamera);
    for (auto it = modelsByDistance.rbegin(); it != modelsByDistance.rend();
         ++it) {
        std::shared_ptr<Model> &model = it->second;
        if (!model->visible) {
            continue;
        }

        bool hasBorder = model->borderSize > 0.0f;
        if (hasBorder) {
            glStencilMask(0xFF);
            glClear(GL_STENCIL_BUFFER_BIT);

            this->drawModel(renderCamera, model.get(), model->shader.get(),
                            projectionMatrix, viewMatrix);

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);

            this->borderShader->use();
            this->borderShader->setFloat("extraOffset", model->borderSize);
            this->borderShader->setVector("color", model->borderColor.x,
                                          model->borderColor.y,
                                          model->borderColor.z);
            this->drawModel(renderCamera, model.get(), this->borderShader.get(),
                            projectionMatrix, viewMatrix);

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0x00);
        } else {
            glStencilMask(0x00);
            this->drawModel(renderCamera, model.get(), model->shader.get(),
                            projectionMatrix, viewMatrix);
        }
    }

    for (size_t i = 0; i < this->pointLights.size(); i++) {
        this->pointLights.at(i)->drawDebugBillboard();
    }
}

void Scene::drawModel(Camera *renderCamera, Model *model, Shader *shader,
                      glm::mat4 &projectionMatrix, glm::mat4 &viewMatrix) {
    shader->use();
    directionalLight.updateShader("directionalLight", shader);

    shader->setInt("nPointLights", this->pointLights.size());
    for (size_t light = 0; light < this->pointLights.size(); light++) {
        std::string prefix = "pointLights[" + std::to_string(light) + "]";
        this->pointLights.at(light)->updateShader(prefix, shader);
    }

    int nSpotLights = this->spotLights.size();
    if (this->flashlightEnabled) {
        nSpotLights += 1;
    }

    shader->setInt("nSpotLights", nSpotLights);
    for (size_t light = 0; light < this->spotLights.size(); light++) {
        std::string prefix = "spotLights[" + std::to_string(light) + "]";
        this->spotLights.at(light)->updateShader(prefix, shader);
    }

    if (this->flashlightEnabled) {
        std::string flashlightPrefix =
            "spotLights[" + std::to_string(this->spotLights.size()) + "]";
        this->flashlight.updateShader(flashlightPrefix, shader);
    }

    shader->setVector("cameraPosition", renderCamera->location.x,
                      this->camera.location.y, renderCamera->location.z);

    shader->setMatrix("projection", glm::value_ptr(projectionMatrix));
    shader->setMatrix("view", glm::value_ptr(viewMatrix));

    shader->setInt("environment", 7);

    model->draw(shader);
}
