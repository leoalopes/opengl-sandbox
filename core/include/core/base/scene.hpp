#pragma once

#include "core/base/camera.hpp"
#include "core/base/environment.hpp"
#include "core/light/directional_light.hpp"
#include "core/light/point_light.hpp"
#include "core/light/spot_light.hpp"
#include "core/objects/object.hpp"
#include "core/objects/quad_primitive.hpp"
#include "core/objects/texture_2d_renderer.hpp"

#include <map>
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

    Camera camera;
    std::shared_ptr<Texture2D> sceneTexture;
    Texture2DRenderer sceneRenderer;
    std::shared_ptr<Shader> postProcessingShader;
    QuadPrimitive fullscreenQuad;

    std::shared_ptr<Environment> environment;

    DirectionalLight directionalLight;
    std::vector<std::shared_ptr<PointLight>> pointLights;
    std::vector<std::shared_ptr<SpotLight>> spotLights;

    SpotLight flashlight;
    float flashlightToggleCooldown = 0;
    bool flashlightEnabled = false;
    bool renderToTexture = false;

    std::vector<std::shared_ptr<Object>> objects;

    std::shared_ptr<Shader> borderShader;

    Scene(unsigned int screenWidth, unsigned int screenHeight,
          std::shared_ptr<Shader> postProcessingShader);

    std::multimap<float, std::shared_ptr<Object>>
    getObjectsByDistance(Camera *renderCamera);

    void draw(int depth = 0);
    void draw(Camera *renderCamera, int width, int height, int depth = 0);
    void drawObject(Camera *renderCamera, Object *object, Shader *shader,
                    glm::mat4 projectionMatrix, glm::mat4 viewMatrix,
                    float time, int depth = 0);
};
