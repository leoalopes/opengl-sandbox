#include "glm/fwd.hpp"
#include <core/interface/window.hpp>

#include <core/base/camera.hpp>
#include <core/base/scene.hpp>
#include <core/graphics/shader.hpp>
#include <core/graphics/texture.hpp>
#include <core/objects/model.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

void processInput(Window &window, Scene *scene);

int main() {
    freopen("execution_log.txt", "a", stdout);

    Window &window = Window::getInstance();
    window.initialize();

    Scene *scene = window.createScene();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::shared_ptr<Shader> borderShader = std::make_shared<Shader>(
        "assets/shaders/highlight.vert", "assets/shaders/solid.frag");
    std::shared_ptr<Shader> standardShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/standard.frag");

    scene->borderShader = borderShader;

    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(1.2f, 2.6f, 2.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(-5.0f, 1.0f, -10.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(0.0f, 1.0f, -7.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene->pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(2.0f, 6.0f, -16.5f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));

    std::shared_ptr<Model> table = std::make_shared<Model>(
        "assets/models/table/SM_OfficeTable.gltf", standardShader);
    table->transform.position.z = -2.0f;
    table->borderSize = 0.1f;
    table->borderColor = glm::vec3(0.0f, 1.0f, 0.0f);

    std::shared_ptr<Model> refrigerator = std::make_shared<Model>(
        "assets/models/refrigerator/SM_Refrigerator.gltf", standardShader);
    refrigerator->transform.position.z = 4.0f;
    refrigerator->borderSize = 0.15f;
    refrigerator->borderColor = glm::vec3(0.5f);

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

    std::shared_ptr<Model> lamppost = std::make_shared<Model>(
        "assets/models/lamppost/scene.gltf", standardShader);
    lamppost->transform.scale = glm::vec3(0.01f);
    lamppost->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    lamppost->borderSize = 0.05f;
    lamppost->borderColor = glm::vec3(1.0f, 0.0f, 0.0f);

    std::shared_ptr<Model> basicWindow = std::make_shared<Model>(
        "assets/models/window/scene.gltf", standardShader);
    basicWindow->transform.rotation = {-90, 0, 0};

    scene->models.push_back(table);
    scene->models.push_back(refrigerator);
    scene->models.push_back(plainWall);
    scene->models.push_back(asphaltWall);
    scene->models.push_back(tiledWall);
    scene->models.push_back(lamppost);
    scene->models.push_back(basicWindow);

    std::shared_ptr<Environment> skybox =
        std::make_shared<Environment>("assets/models/skybox");
    scene->environment = skybox;

    while (!glfwWindowShouldClose(window.glfwWindow)) {
        window.update();
        processInput(window, scene);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);
        scene->draw();

        glfwSwapBuffers(window.glfwWindow);
        glfwPollEvents();
    }

    return 0;
}

void processInput(Window &window, Scene *scene) {
    GLFWwindow *glfwWindow = window.glfwWindow;

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
}
