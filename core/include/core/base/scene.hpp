#pragma once

#include "core/base/camera.hpp"
#include "core/base/environment.hpp"
#include "core/light/directional_light.hpp"
#include "core/light/point_light.hpp"
#include "core/light/spot_light.hpp"
#include "core/objects/model.hpp"

#include <memory>
#include <vector>

constexpr glm::vec3 DEFAULT_DL_DIRECTION = {0.0f, -1.0f, 1.0f};
constexpr glm::vec3 DEFAULT_DL_AMBIENT = {0.15f, 0.135f, 0.125f};
constexpr glm::vec3 DEFAULT_DL_DIFFUSE = {0.5f, 0.465f, 0.375f};
constexpr glm::vec3 DEFAULT_DL_SPECULAR = {0.5f, 0.49f, 0.45f};

class Scene {
  public:
    unsigned int screenWidth;
    unsigned int screenHeight;
    glm::mat4 projectionMatrix;

    Camera camera;

    std::shared_ptr<Environment> environment;

    DirectionalLight directionalLight;
    std::vector<std::shared_ptr<PointLight>> pointLights;
    std::vector<std::shared_ptr<SpotLight>> spotLights;

    SpotLight flashlight;
    bool flashlightEnabled = true;

    std::vector<std::shared_ptr<Model>> models;

    std::shared_ptr<Shader> borderShader;

    Scene(unsigned int screenWidth, unsigned int screenHeight);

    void updateProjectionMatrix(Camera *renderCamera);

    std::multimap<float, std::shared_ptr<Model>>
    getModelsByDistance(Camera *renderCamera);

    void draw();
    void draw(Camera *renderCamera);
    void drawModel(Camera *renderCamera, Model *model, Shader *shader,
                   glm::mat4 &projectionMatrix, glm::mat4 &viewMatrix);
};
