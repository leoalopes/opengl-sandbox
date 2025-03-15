#include <glad/glad.h>

#include "ui/graphical_interface.hpp"
#include <core/base/camera.hpp>
#include <core/base/scene.hpp>
#include <core/graphics/shader.hpp>
#include <core/graphics/texture.hpp>
#include <core/objects/model.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

void ShowPerformanceOverlay(bool *p_open);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float guiToggleCooldown = 0;
float flashlightToggleCooldown = 0;

Scene scene{SCR_WIDTH, SCR_HEIGHT};
GraphicalInterface gui{&scene};

int main() {
    freopen("log.txt", "a", stdout);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Sandbox", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR,
                     gui.showSceneController ? GLFW_CURSOR_NORMAL
                                             : GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    gui.initialize(window);

    std::shared_ptr<Shader> borderShader = std::make_shared<Shader>(
        "assets/shaders/highlight.vert", "assets/shaders/solid.frag");
    std::shared_ptr<Shader> standardShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/standard.frag");

    scene.borderShader = borderShader;

    scene.pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(1.2f, 2.6f, 2.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene.pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(-5.0f, 1.0f, -10.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene.pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(0.0f, 1.0f, -7.0f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));
    scene.pointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(2.0f, 6.0f, -16.5f), glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.09f, 0.032f));

    std::shared_ptr<Model> table = std::make_shared<Model>(
        "assets/models/table/SM_OfficeTable.gltf", standardShader);
    table->transform.translation.z = -2.0f;
    table->borderSize = 0.1f;
    table->borderColor = glm::vec3(0.0f, 1.0f, 0.0f);

    std::shared_ptr<Model> refrigerator = std::make_shared<Model>(
        "assets/models/refrigerator/SM_Refrigerator.gltf", standardShader);
    refrigerator->transform.translation.z = 4.0f;
    refrigerator->borderSize = 0.15f;
    refrigerator->borderColor = glm::vec3(0.5f);

    std::shared_ptr<Model> plainWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Plain.gltf", standardShader);
    plainWall->transform.rotation.x = 90.0f;
    plainWall->transform.translation.x = 1.0f;

    std::shared_ptr<Model> asphaltWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Asphalt.gltf", standardShader);
    asphaltWall->transform.rotation.x = 90.0f;
    asphaltWall->transform.translation.x = -5.0f;

    std::shared_ptr<Model> tiledWall = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Tiled.gltf", standardShader);
    tiledWall->transform.rotation.x = 90.0f;
    tiledWall->transform.translation.x = -11.0f;

    std::shared_ptr<Model> lamppost = std::make_shared<Model>(
        "assets/models/lamppost/scene.gltf", standardShader);
    lamppost->transform.scale = glm::vec3(0.01f);
    lamppost->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
    lamppost->borderSize = 0.05f;
    lamppost->borderColor = glm::vec3(1.0f, 0.0f, 0.0f);

    scene.models.push_back(table);
    scene.models.push_back(refrigerator);
    scene.models.push_back(plainWall);
    scene.models.push_back(asphaltWall);
    scene.models.push_back(tiledWall);
    scene.models.push_back(lamppost);

    std::shared_ptr<Environment> skybox =
        std::make_shared<Environment>("assets/models/skybox");
    scene.environment = skybox;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        flashlightToggleCooldown -= deltaTime;
        if (flashlightToggleCooldown < 0) {
            flashlightToggleCooldown = 0;
        }

        guiToggleCooldown -= deltaTime;
        if (guiToggleCooldown < 0) {
            guiToggleCooldown = 0;
        }

        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

        scene.draw();

        gui.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gui.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (guiToggleCooldown <= 0) {
            gui.acceptEvents = !gui.showSceneController;
            gui.showSceneController = !gui.showSceneController;
            glfwSetInputMode(window, GLFW_CURSOR,
                             gui.showSceneController ? GLFW_CURSOR_NORMAL
                                                     : GLFW_CURSOR_DISABLED);
            guiToggleCooldown = 0.5f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && gui.acceptEvents) {
        if (guiToggleCooldown <= 0) {
            gui.showDemoWindow = !gui.showDemoWindow;
            guiToggleCooldown = 0.5f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (flashlightToggleCooldown <= 0) {
            scene.flashlightEnabled = !scene.flashlightEnabled;
            flashlightToggleCooldown = 0.5f;
        }
    }

    if (gui.acceptEvents) {
        return;
    }

    glm::vec3 cameraPosition = scene.camera.getLocation();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += scene.camera.getForwardVector() * 5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition += scene.camera.getForwardVector() * -5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition += scene.camera.getRightVector() * -5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += scene.camera.getRightVector() * 5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPosition += scene.camera.getUpVector() * 5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraPosition += scene.camera.getUpVector() * -5.0f * deltaTime;
        scene.camera.setLocation(cameraPosition);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xPos = static_cast<float>(xposIn);
    float yPos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset =
        lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    xOffset *= 0.1f;
    yOffset *= 0.1f;

    if (!gui.acceptEvents) {
        scene.camera.updatePitchYaw(xOffset, yOffset);
    }
}
