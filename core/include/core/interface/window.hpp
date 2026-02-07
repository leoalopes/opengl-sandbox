#pragma once
#include <glad/glad.h>

#include "core/base/scene.hpp"

#include <GLFW/glfw3.h>
#include <memory>

class Window;
typedef void (*FramebufferSizeCallback)(Window &window, int width, int height);
typedef void (*CursorPosCallback)(Window &window, double xpos, double ypos);

class Window {
  public:
    static Window &getInstance() {
        static Window instance;
        return instance;
    }

    Window(Window const &) = delete;
    void operator=(Window const &) = delete;

    GLFWwindow *glfwWindow;
    CursorPosCallback cursorPosCallback = nullptr;
    FramebufferSizeCallback framebufferSizeCallback = nullptr;
    bool inputEnabled = true;

    int screenWidth = 1920, screenHeight = 1080;
    float deltaTime, lastFrame;

    glm::vec2 mouseOffset;
    glm::vec2 mousePosition{-1};

    std::unique_ptr<Scene> scene;

    void initialize();
    void terminate();

    Scene *createScene();
    void update();
    bool shouldClose();

  private:
    Window() = default;
    ~Window() { this->terminate(); }

    static void defaultFramebufferSizeCallback(GLFWwindow *window, int width,
                                               int height);

    static void defaultCursorPosCallback(GLFWwindow *window, double xposIn,
                                         double yposIn);
};
