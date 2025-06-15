#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <core/interface/window.hpp>

#include <core/base/camera.hpp>
#include <core/base/scene.hpp>
#include <core/graphics/shader.hpp>
#include <core/graphics/texture.hpp>
#include <core/objects/model.hpp>
#include <core/objects/object_buffer.hpp>
#include <core/objects/texture_renderer.hpp>

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
    std::shared_ptr<Shader> mirrorShader = std::make_shared<Shader>(
        "assets/shaders/standard.vert", "assets/shaders/mirror.frag");

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
    table->transform.position = glm::vec3(-8.0f, 0.05f, 2.0f);

    std::shared_ptr<Model> refrigerator = std::make_shared<Model>(
        "assets/models/refrigerator/SM_Refrigerator.gltf", standardShader);
    refrigerator->transform.position = glm::vec3(2.0f, 0.1f, 4.0f);

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
    lamppost->transform.position = glm::vec3(-0.5f, 0.1f, 0.5f);

    std::shared_ptr<Model> basicWindow = std::make_shared<Model>(
        "assets/models/window/scene.gltf", standardShader);
    basicWindow->transform.position.x = -2.0f;
    basicWindow->transform.position.z = -1.0f;

    std::shared_ptr<Model> mirror = std::make_shared<Model>(
        "assets/models/wall/SM_Wall_Asphalt.gltf", mirrorShader);
    mirror->transform.position = glm::vec3(2.5f, 2.0f, -2.5f);
    mirror->transform.scale = glm::vec3(0.2f);
    mirror->transform.rotation.z = 180.0f;

    Camera mirrorCamera;

    scene->models.push_back(table);
    scene->models.push_back(refrigerator);
    scene->models.push_back(plainWall);
    scene->models.push_back(asphaltWall);
    scene->models.push_back(tiledWall);
    scene->models.push_back(lamppost);
    scene->models.push_back(basicWindow);
    scene->models.push_back(mirror);

    std::shared_ptr<Environment> skybox =
        std::make_shared<Environment>("assets/models/skybox");
    scene->environment = skybox;

    TextureRenderer renderToTexture{720, 1280};
    ObjectBuffer screenQuad{
        {{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
         {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
         {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
         {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}},
        {0, 1, 2, 2, 3, 0}};
    Shader renderToTextureShader{"assets/shaders/identity.vert",
                                 "assets/shaders/kernel.frag"};

    while (!glfwWindowShouldClose(window.glfwWindow)) {
        window.update();
        processInput(window, scene);

        renderToTexture.texture.use(10, mirrorShader.get(), "material.mirror");

        mirrorCamera.location = mirror->transform.position;
        glm::vec3 cameraToMirror =
            mirror->transform.position - scene->camera.location;
        glm::vec3 mirrorForward{0.0f, 0.0f, 1.0f};
        mirrorCamera.updateCameraVectorsFromForwardVector(
            cameraToMirror -
            2.0f * glm::dot(mirrorForward, cameraToMirror) * mirrorForward);

        renderToTexture.bind();
        glViewport(0, 0, 1280, 720); // Set viewport to match texture size
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

        mirror->visible = false;
        scene->draw(&mirrorCamera);

        renderToTexture.unbind();
        int width, height;
        glfwGetFramebufferSize(window.glfwWindow, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

        mirror->visible = true;
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
