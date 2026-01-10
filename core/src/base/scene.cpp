#include "core/base/scene.hpp"

#include "core/base/camera.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>

Scene::Scene(unsigned int screenWidth, unsigned int screenHeight,
             std::shared_ptr<Shader> postProcessingShader)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      directionalLight(DEFAULT_DL_DIRECTION, DEFAULT_DL_AMBIENT,
                       DEFAULT_DL_DIFFUSE, DEFAULT_DL_SPECULAR),
      flashlight(glm::vec3(0.0f), glm::vec3(0.0f),
                 glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(20.0f)),
                 glm::vec3(0.9f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f),
      sceneTexture(std::make_shared<Texture2D>(
          screenWidth, screenHeight, GL_RGBA, GL_CLAMP_TO_EDGE,
          GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR)),
      sceneRenderer(screenHeight, screenWidth, sceneTexture),
      postProcessingShader(postProcessingShader),
      fullscreenQuad(postProcessingShader, sceneTexture) {};

std::multimap<float, std::shared_ptr<Object>>
Scene::getObjectsByDistance(Camera *renderCamera) {
    std::multimap<float, std::shared_ptr<Object>> objectsByDistance;
    for (const auto &object : this->objects) {
        float distance =
            glm::length(object->transform.position - renderCamera->location);
        objectsByDistance.insert({distance, object});
    }

    return objectsByDistance;
}

void Scene::draw(int depth) {
    glViewport(0, 0, screenWidth, screenHeight);
    this->sceneRenderer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->draw(&this->camera, screenWidth, screenHeight, depth);
    this->sceneRenderer.unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    postProcessingShader->use();
    fullscreenQuad.draw(postProcessingShader.get());
    glEnable(GL_DEPTH_TEST);
}

void Scene::draw(Camera *renderCamera, int width, int height, int depth) {
    glm::mat4 projectionMatrix =
        glm::perspective(glm::radians(renderCamera->fov),
                         (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 viewMatrix = renderCamera->getLookAt();
    if (this->environment) {
        this->environment->draw(viewMatrix, projectionMatrix);
    }

    auto objectsByDistance = this->getObjectsByDistance(renderCamera);
    for (auto it = objectsByDistance.rbegin(); it != objectsByDistance.rend();
         ++it) {
        std::shared_ptr<Object> &object = it->second;
        if (!object->visible) {
            continue;
        }

        flashlight.position = renderCamera->location;
        flashlight.direction = renderCamera->forwardVector;

        this->drawObject(renderCamera, object.get(), object->shader.get(),
                         projectionMatrix, viewMatrix, depth);
    }

    for (size_t i = 0; i < this->pointLights.size(); i++) {
        /*this->pointLights.at(i)->drawDebugBillboard();*/
    }
}

void Scene::drawObject(Camera *renderCamera, Object *object, Shader *shader,
                       glm::mat4 projectionMatrix, glm::mat4 viewMatrix,
                       int depth) {
    if (object->useDynamicEnvironmentMap && depth == 0) {
        bool originalFlashlightEnabled = flashlightEnabled;
        glm::vec3 originalFlashlightPosition = flashlight.position;
        glm::vec3 originalFlashlightDirection = flashlight.direction;

        flashlightEnabled = false;

        glm::vec3 temporaryCameraPosition =
            object->transform.getMatrix(-object->centerPoint) *
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        Camera temporaryCamera{
            temporaryCameraPosition, {0.0f, 1.0f, 0.0f}, 90.0f};
        object->visible = false;

        glm::vec3 forwardVector{0.0f, 0.0f, -1.0f};
        glm::vec3 rightVector{1.0f, 0.0f, 0.0f};
        glm::vec3 upVector{0.0f, 1.0f, 0.0f};

        glViewport(0, 0, object->dynamicEnvironmentMapTextureTarget->width,
                   object->dynamicEnvironmentMapTextureTarget->height);

        // GL_TEXTURE_CUBE_MAP_POSITIVE_X (+X face) - Looking towards +X
        object->dynamicEnvironmentMapRenderer->bind(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(rightVector,
                                                             -upVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        // GL_TEXTURE_CUBE_MAP_NEGATIVE_X (-X face) - Looking towards -X
        object->dynamicEnvironmentMapRenderer->bind(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(-rightVector,
                                                             -upVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        // GL_TEXTURE_CUBE_MAP_POSITIVE_Y (+Y face) - Looking towards +Y
        object->dynamicEnvironmentMapRenderer->bind(2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(upVector,
                                                             -forwardVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y (-Y face) - Looking towards -Y
        object->dynamicEnvironmentMapRenderer->bind(3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(-upVector,
                                                             forwardVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        // GL_TEXTURE_CUBE_MAP_POSITIVE_Z (+Z face) - Looking towards +Z
        object->dynamicEnvironmentMapRenderer->bind(4);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(-forwardVector,
                                                             -upVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z (-Z face) - Looking towards -Z
        object->dynamicEnvironmentMapRenderer->bind(5);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        temporaryCamera.updateCameraVectorsFromForwardVector(forwardVector,
                                                             -upVector);
        this->draw(
            &temporaryCamera, object->dynamicEnvironmentMapTextureTarget->width,
            object->dynamicEnvironmentMapTextureTarget->height, depth + 1);

        object->visible = true;
        object->dynamicEnvironmentMapRenderer->unbind();

        glActiveTexture(GL_TEXTURE0 + 8);
        object->dynamicEnvironmentMapTextureTarget->bind();

        glViewport(0, 0, screenWidth, screenHeight);
        this->sceneRenderer.bind();

        flashlightEnabled = originalFlashlightEnabled;
        flashlight.position = originalFlashlightPosition;
        flashlight.direction = originalFlashlightDirection;
    }

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
                      renderCamera->location.y, renderCamera->location.z);

    shader->setMatrix("projection", glm::value_ptr(projectionMatrix));
    shader->setMatrix("view", glm::value_ptr(viewMatrix));

    shader->setInt("environment", object->useDynamicEnvironmentMap ? 8 : 7);

    object->draw(shader);
}
