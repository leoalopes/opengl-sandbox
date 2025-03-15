#pragma once

#include "core/base/transform.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/objects/mesh.hpp"

#include <memory>
#include <string>
#include <tiny_gltf/tiny_gltf.h>

class Model {
  public:
    Transform transform;
    std::shared_ptr<Shader> shader;

    float borderSize = 0.0f;
    glm::vec3 borderColor;

    Model(std::string path, std::shared_ptr<Shader> shader)
        : path(PROJECT_DIRECTORY + path), shader(shader) {
        this->loadModel();
    };

    void draw();
    void draw(Shader *overrideShader);

  private:
    std::string path;
    tinygltf::Model tgModel;
    tinygltf::TinyGLTF tgLoader;
    std::map<unsigned int, Texture> textures;
    std::vector<Mesh> meshes;

    void loadModel();

    void processTexture(const tinygltf::Texture &texture, int index);
    void processNode(const tinygltf::Node &node);
    void processMesh(const tinygltf::Mesh &mesh);
};
