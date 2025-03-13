#pragma once

#include <memory>
#include <tiny_gltf/tiny_gltf.h>

#include "core/mesh.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "core/transform.hpp"
#include <string>

class Model {
  public:
    Transform transform;
    std::shared_ptr<Shader> shader;

    float borderSize = 0.0f;
    glm::vec3 borderColor;

    Model(std::string path, std::shared_ptr<Shader> shader)
        : path(path), shader(shader) {
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
