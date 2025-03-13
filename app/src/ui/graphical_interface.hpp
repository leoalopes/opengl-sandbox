#pragma once
#include "core/scene.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

class GraphicalInterface {
  public:
    Scene *scene;

    bool acceptEvents = false;
    bool showDemoWindow = false;
    bool showSceneController = false;
    bool showPerformanceOverlay = true;

    GraphicalInterface(Scene *scene) : scene(scene) {};

    void initialize(GLFWwindow *window);
    void shutdown();

    void draw();

  private:
    void drawSceneController();
    void drawPerformanceOverlay(float framerate);
};
