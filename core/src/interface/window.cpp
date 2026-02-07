#include "core/interface/window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <stdexcept>

void Window::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    this->glfwWindow = glfwCreateWindow(screenWidth, screenHeight,
                                        "OpenGL Sandbox", NULL, NULL);
    if (glfwWindow == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(this->glfwWindow);
    glfwSetFramebufferSizeCallback(this->glfwWindow,
                                   this->defaultFramebufferSizeCallback);
    glfwSetCursorPosCallback(this->glfwWindow, this->defaultCursorPosCallback);
    glfwSetInputMode(this->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::terminate() {
    if (this->glfwWindow != NULL) {
        glfwDestroyWindow(this->glfwWindow);
    }
    glfwTerminate();
}

Scene *Window::createScene() {
    std::shared_ptr<Shader> postProcessingShader = std::make_shared<Shader>(
        "assets/shaders/identity.vert", "assets/shaders/gamma.frag");
    this->scene = std::make_unique<Scene>(this->screenWidth, this->screenHeight,
                                          postProcessingShader);
    return this->scene.get();
}

void Window::update() {
    float currentFrame = static_cast<float>(glfwGetTime());
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;

    glm::vec3 cameraTranslation{0.0f};
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
        cameraTranslation += this->scene->camera.forwardVector;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
        cameraTranslation -= this->scene->camera.forwardVector;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
        cameraTranslation += this->scene->camera.rightVector;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
        cameraTranslation -= this->scene->camera.rightVector;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraTranslation += this->scene->camera.upVector;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraTranslation -= this->scene->camera.upVector;
    }

    float translationLength = glm::length(cameraTranslation);
    if (translationLength > 0) {
        this->scene->camera.location +=
            (cameraTranslation / translationLength) * 5.0f * this->deltaTime;
    }
}

void Window::defaultFramebufferSizeCallback(GLFWwindow *window, int width,
                                            int height) {
    Window &windowInstance = Window::getInstance();
    glViewport(0, 0, width, height);
    if (windowInstance.framebufferSizeCallback) {
        windowInstance.framebufferSizeCallback(windowInstance, width, height);
    }
}

void Window::defaultCursorPosCallback(GLFWwindow *window, double xpos,
                                      double ypos) {
    Window &windowInstance = Window::getInstance();
    glm::vec2 newPosition{static_cast<float>(xpos), static_cast<float>(ypos)};

    if (windowInstance.mousePosition.x == -1 &&
        windowInstance.mousePosition.y == -1) {
        windowInstance.mousePosition = newPosition;
    }

    windowInstance.mouseOffset = {
        newPosition.x - windowInstance.mousePosition.x,
        windowInstance.mousePosition.y - newPosition.y};
    windowInstance.mousePosition = newPosition;

    glm::vec2 cameraMouseOffset = windowInstance.mouseOffset * 0.1f;
    if (windowInstance.scene) {
        windowInstance.scene->camera.updatePitchYaw(cameraMouseOffset.x,
                                                    cameraMouseOffset.y);
    }

    if (windowInstance.cursorPosCallback) {
        windowInstance.cursorPosCallback(windowInstance, xpos, ypos);
    }
}
