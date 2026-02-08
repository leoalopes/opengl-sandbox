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

float toggleCooldown = 0.0f;

void loadModels(Scene *scene);
void processInput(Window &window, GraphicalInterface &gui, Scene *scene);

int main() {
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
    scene->borderShader = borderShader;

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>(
        "assets/shaders/origin.vert", "assets/shaders/round_point.frag");
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
    scene->spotLights.push_back(std::make_shared<SpotLight>(
        glm::vec3(-2.5f, 2.6f, 2.0f), glm::vec3(-0.4f, -1.0f, -0.4f),
        glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(20.0f)),
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f,
        0.032f, lightShader));

    //
    // std::shared_ptr<Model> basicWindow = std::make_shared<Model>(
    //     "assets/models/window/scene.gltf", standardShader);
    // basicWindow->transform.position.x = -2.0f;
    // basicWindow->transform.position.z = -1.0f;
    //

    // scene->objects.push_back(table);
    // scene->objects.push_back(refrigerator);
    // scene->objects.push_back(plainWall);
    // scene->objects.push_back(asphaltWall);
    // scene->objects.push_back(tiledWall);
    // scene->objects.push_back(basicWindow);
    // scene->objects.push_back(sphere);
    loadModels(scene);

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

        toggleCooldown = std::max(0.0f, toggleCooldown - window.deltaTime);
        processInput(window, gui, scene);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->draw();

        gui.draw();

        glfwSwapBuffers(window.glfwWindow);
    }

    return 0;
}

void loadModels(Scene *scene) {
    std::shared_ptr<Shader> standardShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/standard.frag");
    std::shared_ptr<Shader> expandingShader = std::make_shared<Shader>(
        "assets/shaders/standard-with-geom.vert",
        "assets/shaders/standard.frag", "assets/shaders/expand.geom");
    std::shared_ptr<Shader> mirrorShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/mirror.frag");
    std::shared_ptr<Shader> solidShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/solid.frag");

    std::shared_ptr<Model> refrigerator = std::make_shared<Model>(
        "assets/models/refrigerator/SM_Refrigerator.gltf", standardShader);
    refrigerator->transform.position.x = -0.15f;
    refrigerator->transform.position.y = -1.25f;
    refrigerator->transform.position.z = 4.5f;

    std::shared_ptr<Model> refrigerator2 =
        std::make_shared<Model>(refrigerator.get());
    refrigerator2->transform.position.x = -14.15f;
    refrigerator2->transform.position.y = -1.25f;
    refrigerator2->transform.position.z = 4.5f;

    std::shared_ptr<Model> refrigerator3 =
        std::make_shared<Model>(refrigerator.get());
    refrigerator3->transform.rotation.y = 180.0f;
    refrigerator3->transform.position.x = -0.15f;
    refrigerator3->transform.position.y = -1.25f;
    refrigerator3->transform.position.z = -9.0f;

    std::shared_ptr<Model> refrigerator4 =
        std::make_shared<Model>(refrigerator.get());
    refrigerator4->transform.rotation.y = 180.0f;
    refrigerator4->transform.position.x = -14.15f;
    refrigerator4->transform.position.y = -1.25f;
    refrigerator4->transform.position.z = -9.0f;

    scene->objects.push_back(refrigerator);
    scene->objects.push_back(refrigerator2);
    scene->objects.push_back(refrigerator3);
    scene->objects.push_back(refrigerator4);

    std::shared_ptr<Model> table = std::make_shared<Model>(
        "assets/models/table/SM_OfficeTable.gltf", standardShader);
    table->transform.rotation.y = -90.0f;
    table->transform.position.x = -4.5f;
    table->transform.position.y = -1.9f;
    table->transform.position.z = 0.5f;

    std::shared_ptr<Model> table2 = std::make_shared<Model>(table.get());
    table2->transform.rotation.y = -90.0f;
    table2->transform.position.x = -4.5f;
    table2->transform.position.y = -1.9f;
    table2->transform.position.z = -4.5f;

    std::shared_ptr<Model> table3 = std::make_shared<Model>(table.get());
    table3->transform.rotation.y = 90.0f;
    table3->transform.position.x = -9.5f;
    table3->transform.position.y = -1.9f;
    table3->transform.position.z = -4.5f;

    std::shared_ptr<Model> table4 = std::make_shared<Model>(table.get());
    table4->transform.rotation.y = 90.0f;
    table4->transform.position.x = -4.5f;
    table4->transform.position.y = -1.9f;
    table4->transform.position.z = 0.5f;

    scene->objects.push_back(table);
    scene->objects.push_back(table2);
    scene->objects.push_back(table3);
    scene->objects.push_back(table4);

    std::shared_ptr<Model> window = std::make_shared<Model>(
        "assets/models/window/scene.gltf", standardShader);
    window->transform.rotation.y = 180.0f;
    window->transform.position.x = -2.0f;
    window->transform.position.y = -1.0f;
    window->transform.position.z = -2.2f;

    std::shared_ptr<Model> window2 = std::make_shared<Model>(window.get());
    window2->transform.rotation.y = -90.0f;
    window2->transform.position.x = -7.2f;
    window2->transform.position.y = -1.0f;
    window2->transform.position.z = -7.0f;

    std::shared_ptr<Model> window3 = std::make_shared<Model>(window.get());
    window3->transform.rotation.y = 360.0f;
    window3->transform.position.x = -12.0f;
    window3->transform.position.y = -1.0f;
    window3->transform.position.z = -1.8f;

    std::shared_ptr<Model> window4 = std::make_shared<Model>(window.get());
    window4->transform.rotation.y = 90.0f;
    window4->transform.position.x = -7.2f;
    window4->transform.position.y = -1.0f;
    window4->transform.position.z = 3.0f;

    scene->objects.push_back(window);
    scene->objects.push_back(window2);
    scene->objects.push_back(window3);
    scene->objects.push_back(window4);

    std::shared_ptr<Model> groundPlain = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Plain.gltf", standardShader);
    groundPlain->transform.rotation.x = 90.0f;
    groundPlain->transform.position.y = -2.0f;

    std::shared_ptr<Model> groundPlain2 =
        std::make_shared<Model>(groundPlain.get());
    groundPlain2->transform.rotation.x = 90.0f;
    groundPlain2->transform.position.x = -5.0f;
    groundPlain2->transform.position.y = -2.0f;

    std::shared_ptr<Model> groundPlain3 =
        std::make_shared<Model>(groundPlain.get());
    groundPlain3->transform.rotation.x = 90.0f;
    groundPlain3->transform.position.x = -5.0f;
    groundPlain3->transform.position.y = -2.0f;
    groundPlain3->transform.position.z = 5.0f;

    std::shared_ptr<Model> groundPlain4 =
        std::make_shared<Model>(groundPlain.get());
    groundPlain4->transform.rotation.x = 90.0f;
    groundPlain4->transform.position.y = -2.0f;
    groundPlain4->transform.position.z = 5.0f;

    std::shared_ptr<Model> groundPlain5 =
        std::make_shared<Model>(groundPlain.get());
    groundPlain5->transform.rotation.x = 90.0f;
    groundPlain5->transform.position.y = -2.0f;
    groundPlain5->transform.position.z = -5.0f;

    std::shared_ptr<Model> groundPlain6 =
        std::make_shared<Model>(groundPlain.get());
    groundPlain6->transform.rotation.x = 90.0f;
    groundPlain6->transform.position.x = -5.0f;
    groundPlain6->transform.position.y = -2.0f;
    groundPlain6->transform.position.z = -5.0f;

    scene->objects.push_back(groundPlain);
    scene->objects.push_back(groundPlain2);
    scene->objects.push_back(groundPlain3);
    scene->objects.push_back(groundPlain4);
    scene->objects.push_back(groundPlain5);
    scene->objects.push_back(groundPlain6);

    std::shared_ptr<Model> groundAsphalt = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Asphalt.gltf", standardShader);
    groundAsphalt->transform.rotation.x = 90.0f;
    groundAsphalt->transform.position.x = -10.0f;
    groundAsphalt->transform.position.y = -2.0f;

    std::shared_ptr<Model> groundAsphalt2 =
        std::make_shared<Model>(groundAsphalt.get());
    groundAsphalt2->transform.rotation.x = 90.0f;
    groundAsphalt2->transform.position.x = -10.0f;
    groundAsphalt2->transform.position.y = -2.0f;
    groundAsphalt2->transform.position.z = 5.0f;

    std::shared_ptr<Model> groundAsphalt3 =
        std::make_shared<Model>(groundAsphalt.get());
    groundAsphalt3->transform.rotation.x = 90.0f;
    groundAsphalt3->transform.position.x = -15.0f;
    groundAsphalt3->transform.position.y = -2.0f;

    std::shared_ptr<Model> groundAsphalt4 =
        std::make_shared<Model>(groundAsphalt.get());
    groundAsphalt4->transform.rotation.x = 90.0f;
    groundAsphalt4->transform.position.x = -15.0f;
    groundAsphalt4->transform.position.y = -2.0f;
    groundAsphalt4->transform.position.z = 5.0f;

    scene->objects.push_back(groundAsphalt);
    scene->objects.push_back(groundAsphalt2);
    scene->objects.push_back(groundAsphalt3);
    scene->objects.push_back(groundAsphalt4);

    std::shared_ptr<Model> groundTiled = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Tiled.gltf", standardShader);
    groundTiled->transform.rotation.x = 90.0f;
    groundTiled->transform.position.x = -10.0f;
    groundTiled->transform.position.y = -2.0f;
    groundTiled->transform.position.z = -5.0f;

    std::shared_ptr<Model> groundTiled2 =
        std::make_shared<Model>(groundTiled.get());
    groundTiled2->transform.rotation.x = 90.0f;
    groundTiled2->transform.position.x = -10.0f;
    groundTiled2->transform.position.y = -2.0f;
    groundTiled2->transform.position.z = -10.0f;

    std::shared_ptr<Model> groundTiled3 =
        std::make_shared<Model>(groundTiled.get());
    groundTiled3->transform.rotation.x = 90.0f;
    groundTiled3->transform.position.x = -15.0f;
    groundTiled3->transform.position.y = -2.0f;
    groundTiled3->transform.position.z = -5.0f;

    std::shared_ptr<Model> groundTiled4 =
        std::make_shared<Model>(groundTiled.get());
    groundTiled4->transform.rotation.x = 90.0f;
    groundTiled4->transform.position.x = -15.0f;
    groundTiled4->transform.position.y = -2.0f;
    groundTiled4->transform.position.z = -10.0f;

    std::shared_ptr<Model> groundTiled5 =
        std::make_shared<Model>(groundTiled.get());
    groundTiled5->transform.rotation.x = 90.0f;
    groundTiled5->transform.position.x = -5.0f;
    groundTiled5->transform.position.y = -2.0f;
    groundTiled5->transform.position.z = -10.0f;

    std::shared_ptr<Model> groundTiled6 =
        std::make_shared<Model>(groundTiled.get());
    groundTiled6->transform.rotation.x = 90.0f;
    groundTiled6->transform.position.y = -2.0f;
    groundTiled6->transform.position.z = -10.0f;

    scene->objects.push_back(groundTiled);
    scene->objects.push_back(groundTiled2);
    scene->objects.push_back(groundTiled3);
    scene->objects.push_back(groundTiled4);
    scene->objects.push_back(groundTiled5);
    scene->objects.push_back(groundTiled6);

    std::shared_ptr<Texture2D> sphereTexture;
    std::shared_ptr<SpherePrimitive> sphericalMirror =
        std::make_shared<SpherePrimitive>(mirrorShader, sphereTexture);
    sphericalMirror->setupDynamicEnvironmentMap();
    sphericalMirror->transform.position.x = -7.0f;
    sphericalMirror->transform.position.y = -0.5f;
    sphericalMirror->transform.position.z = -2.0f;

    scene->objects.push_back(sphericalMirror);
}

void processInput(Window &window, GraphicalInterface &gui, Scene *scene) {
    GLFWwindow *glfwWindow = window.glfwWindow;

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_L) == GLFW_PRESS) {
        if (toggleCooldown <= 0) {
            gui.acceptEvents = !gui.showLightController;
            gui.showLightController = !gui.showLightController;
            gui.showObjectController = false;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR,
                             gui.acceptEvents ? GLFW_CURSOR_NORMAL
                                              : GLFW_CURSOR_DISABLED);
            toggleCooldown = 0.5f;
            window.inputEnabled = !gui.acceptEvents;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_O) == GLFW_PRESS) {
        if (toggleCooldown <= 0) {
            gui.acceptEvents = !gui.showObjectController;
            gui.showObjectController = !gui.showObjectController;
            gui.showLightController = false;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR,
                             gui.acceptEvents ? GLFW_CURSOR_NORMAL
                                              : GLFW_CURSOR_DISABLED);
            toggleCooldown = 0.5f;
            window.inputEnabled = !gui.acceptEvents;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_I) == GLFW_PRESS && gui.acceptEvents) {
        if (toggleCooldown <= 0) {
            gui.acceptEvents = !gui.showDemoWindow;
            gui.showDemoWindow = !gui.showDemoWindow;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR,
                             gui.acceptEvents ? GLFW_CURSOR_NORMAL
                                              : GLFW_CURSOR_DISABLED);
            toggleCooldown = 0.5f;
            window.inputEnabled = !gui.acceptEvents;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_F) == GLFW_PRESS) {
        if (toggleCooldown <= 0) {
            scene->flashlightEnabled = !scene->flashlightEnabled;
            toggleCooldown = 0.5f;
        }
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS) {
        if (toggleCooldown <= 0) {
            scene->debugLights = !scene->debugLights;
            toggleCooldown = 0.5f;
        }
    }
}
