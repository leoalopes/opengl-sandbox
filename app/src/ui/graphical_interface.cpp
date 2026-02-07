#include "graphical_interface.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

void GraphicalInterface::initialize(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GraphicalInterface::draw() {
    ImGuiIO &io = ImGui::GetIO();
    io.SetAppAcceptingEvents(this->acceptEvents);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (this->showDemoWindow) {
        ImGui::ShowDemoWindow(&this->showDemoWindow);
    }

    this->drawSceneController();
    this->drawPerformanceOverlay(io.Framerate);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicalInterface::drawSceneController() {
    if (!this->showSceneController) {
        return;
    }

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    float padding = 5.0f;
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImGui::SetNextWindowSize(ImVec2(400.0f, viewportSize.y - padding * 2));
    ImGui::SetNextWindowPos(ImVec2(viewportSize.x - 400.0f - padding, padding));

    if (ImGui::Begin("Scene controller", NULL, flags)) {
        if (ImGui::CollapsingHeader("Directional light",
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SeparatorText("Color");

            ImGui::Text("Ambient ");
            ImGui::SameLine();
            ImGui::ColorEdit3("##dl_ambient",
                              &this->scene->directionalLight.ambient.x);

            ImGui::Text("Diffuse ");
            ImGui::SameLine();
            ImGui::ColorEdit3("##dl_diffuse",
                              &this->scene->directionalLight.diffuse.x);

            ImGui::Text("Specular");
            ImGui::SameLine();
            ImGui::ColorEdit3("##dl_specular",
                              &this->scene->directionalLight.specular.x);
            ImGui::Spacing();

            ImGui::SeparatorText("Direction");

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("##dl_dir_x",
                             &this->scene->directionalLight.direction.x, 0.1f,
                             -1.0f, 1.0f, "%.2f");

            ImGui::SameLine();
            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("##dl_dir_y",
                             &this->scene->directionalLight.direction.y, 0.1f,
                             -1.0f, 1.0f, "%.2f");

            ImGui::SameLine();
            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("##dl_dir_z",
                             &this->scene->directionalLight.direction.z, 0.1f,
                             -1.0f, 1.0f, "%.2f");
            ImGui::Spacing();
        }

        if (ImGui::CollapsingHeader("Point lights",
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            for (size_t i = 0; i < this->scene->pointLights.size(); i++) {
                std::string lightName = std::to_string(i + 1);
                if (ImGui::CollapsingHeader(lightName.c_str(),
                                            ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SeparatorText("Color");

                    ImGui::Text("Diffuse ");
                    ImGui::SameLine();
                    ImGui::ColorEdit3(
                        ("##pl_" + lightName + "_diffuse").c_str(),
                        &this->scene->pointLights[i]->diffuse.x);

                    ImGui::Text("Specular");
                    ImGui::SameLine();
                    ImGui::ColorEdit3(
                        ("##pl_" + lightName + "_specular").c_str(),
                        &this->scene->pointLights[i]->specular.x);
                    ImGui::Spacing();

                    ImGui::SeparatorText("Position");

                    ImGui::Text("X");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat(("##pl_" + lightName + "_pos_x").c_str(),
                                     &this->scene->pointLights[i]->position.x,
                                     0.1f);

                    ImGui::SameLine();
                    ImGui::Text("Y");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat(("##pl_" + lightName + "_pos_y").c_str(),
                                     &this->scene->pointLights[i]->position.y,
                                     0.1f);

                    ImGui::SameLine();
                    ImGui::Text("Z");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat(("##pl_" + lightName + "_pos_z").c_str(),
                                     &this->scene->pointLights[i]->position.z,
                                     0.1f);
                    ImGui::Spacing();
                }
            }
            ImGui::Unindent();
        }
    }

    ImGui::End();
}

void GraphicalInterface::drawPerformanceOverlay(float framerate) {
    if (!this->showPerformanceOverlay) {
        return;
    }

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f), ImGuiCond_Always);

    if (ImGui::Begin("Performance Overlay", &this->showPerformanceOverlay,
                     windowFlags)) {
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
    }
    ImGui::End();
}

void GraphicalInterface::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
