#pragma once
#include <core/base/scene.hpp>

#include <GLFW/glfw3.h>
#include <imgui.h>

class GraphicalInterface {
  public:
    Scene *scene;

    bool acceptEvents = false;
    bool showDemoWindow = false;
    bool showLightController = false;
    bool showObjectController = false;
    bool showPerformanceOverlay = true;

    GraphicalInterface(Scene *scene) : scene(scene) {};
    ~GraphicalInterface() { this->shutdown(); };

    void initialize(GLFWwindow *window);
    void shutdown();

    void draw();

  private:
    void drawLightController();
    void drawObjectController();
    void drawPerformanceOverlay(float framerate);
};
