#include <core/base/camera.hpp>
#include <core/base/scene.hpp>
#include <core/graphics/shader.hpp>
#include <core/graphics/texture.hpp>
#include <core/interface/window.hpp>
#include <core/objects/cube_primitive.hpp>
#include <core/objects/model.hpp>
#include <core/objects/object_buffer.hpp>
#include <core/objects/sphere_primitive.hpp>
#include <core/objects/texture_renderer.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "ui/graphical_interface.hpp"

void processInput(Window &window, GraphicalInterface &gui, Scene *scene);

int main() {
    freopen("execution_log.txt", "a", stdout);

    Window &window = Window::getInstance();
    window.initialize();

    Scene *scene = window.createScene();
    scene->renderToTexture = true;

    GraphicalInterface gui{scene};
    gui.initialize(window.glfwWindow);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::shared_ptr<Shader> borderShader = std::make_shared<Shader>(
        "assets/shaders/highlight.vert", "assets/shaders/solid.frag");
    std::shared_ptr<Shader> standardShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/standard.frag");
    std::shared_ptr<Shader> explodingShader = std::make_shared<Shader>(
        "assets/shaders/standard-with-geom.vert",
        "assets/shaders/standard.frag", "assets/shaders/explode.geom");
    std::shared_ptr<Shader> mirrorShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/mirror.frag");
    std::shared_ptr<Shader> solidShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/solid.frag");
    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>(
        "assets/shaders/origin.vert", "assets/shaders/round_point.frag");

    scene->borderShader = borderShader;

    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(1.2f, 2.6f, 2.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f, lightShader));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(-5.0f, 1.0f, -10.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f, lightShader));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(0.0f, 1.0f, -7.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f, lightShader));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(2.0f, 6.0f, -16.5f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f, lightShader));

    std::shared_ptr<Model> table = std::make_shared<Model>(
        "assets/models/table/SM_OfficeTable.gltf", standardShader);
    table->transform.position = glm::vec3(-8.0f, 0.05f, 2.0f);

    std::shared_ptr<Model> refrigerator = std::make_shared<Model>(
        "assets/models/refrigerator/SM_Refrigerator.gltf", explodingShader);
    refrigerator->transform.position = glm::vec3(2.0f, 0.1f, 4.5f);

    std::shared_ptr<Model> plainWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Plain.gltf", standardShader);
    plainWall->transform.rotation.x = 90.0f;
    plainWall->transform.position.x = 1.0f;

    std::shared_ptr<Model> asphaltWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Asphalt.gltf", standardShader);
    asphaltWall->transform.rotation.x = 90.0f;
    asphaltWall->transform.position.x = -5.0f;

    std::shared_ptr<Model> tiledWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Tiled.gltf", standardShader);
    tiledWall->transform.rotation.x = 90.0f;
    tiledWall->transform.position.x = -11.0f;

    std::shared_ptr<Model> basicWindow = std::make_shared<Model>(
        "assets/models/window/scene.gltf", standardShader);
    basicWindow->transform.position.x = -2.0f;
    basicWindow->transform.position.z = -1.0f;

    std::shared_ptr<Texture2D> sphereTexture;
    std::shared_ptr<SpherePrimitive> sphere =
        std::make_shared<SpherePrimitive>(mirrorShader, sphereTexture);
    sphere->setupDynamicEnvironmentMap();
    sphere->transform.position = glm::vec3(2.5f, 2.0f, 2.5f);

    Camera mirrorCamera;

    scene->objects.push_back(table);
    scene->objects.push_back(refrigerator);
    scene->objects.push_back(plainWall);
    scene->objects.push_back(asphaltWall);
    scene->objects.push_back(tiledWall);
    scene->objects.push_back(basicWindow);
    scene->objects.push_back(sphere);

    std::array<std::string, 6> skyboxPaths{
        "assets/models/skybox/right.jpg", "assets/models/skybox/left.jpg",
        "assets/models/skybox/top.jpg",   "assets/models/skybox/bottom.jpg",
        "assets/models/skybox/front.jpg", "assets/models/skybox/back.jpg",
    };
    std::shared_ptr<Environment> skybox =
        std::make_shared<Environment>(skyboxPaths);
    scene->environment = skybox;

    while (!glfwWindowShouldClose(window.glfwWindow)) {
        glfwPollEvents();
        window.update();

        gui.toggleCooldown =
            std::max(0.0f, gui.toggleCooldown - window.deltaTime);
        scene->debugLightsToggleCooldown =
            std::max(0.0f, scene->debugLightsToggleCooldown - window.deltaTime);
        scene->flashlightToggleCooldown =
            std::max(0.0f, scene->flashlightToggleCooldown - window.deltaTime);
        processInput(window, gui, scene);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->draw();

        gui.draw();

        glfwSwapBuffers(window.glfwWindow);
    }

    return 0;
}

void processInput(Window &window, GraphicalInterface &gui, Scene *scene) {
    GLFWwindow *glfwWindow = window.glfwWindow;

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (gui.toggleCooldown <= 0) {
            gui.acceptEvents = !gui.showSceneController;
            gui.showSceneController = !gui.showSceneController;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR,
                             gui.showSceneController ? GLFW_CURSOR_NORMAL
                                                     : GLFW_CURSOR_DISABLED);
            gui.toggleCooldown = 0.5f;
            window.inputEnabled = !gui.acceptEvents;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS && gui.acceptEvents) {
        if (gui.toggleCooldown <= 0) {
            gui.showDemoWindow = !gui.showDemoWindow;
            gui.toggleCooldown = 0.5f;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_F) == GLFW_PRESS) {
        if (scene->flashlightToggleCooldown <= 0) {
            scene->flashlightEnabled = !scene->flashlightEnabled;
            scene->flashlightToggleCooldown = 0.5f;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_L) == GLFW_PRESS) {
        if (scene->debugLightsToggleCooldown <= 0) {
            scene->debugLights = !scene->debugLights;
            scene->debugLightsToggleCooldown = 0.5f;
        }
    }
}
