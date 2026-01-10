#pragma once
#include <core/base/scene.hpp>

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
    ~GraphicalInterface() { this->shutdown(); };

    void initialize(GLFWwindow *window);
    void shutdown();

    void draw();

  private:
    void drawSceneController();
    void drawPerformanceOverlay(float framerate);
};
