#pragma once

#include "core/graphics/shader.hpp"
#include "core/objects/object.hpp"
#include "glm/fwd.hpp"

#include <memory>
#include <string>
#include <tiny_gltf/tiny_gltf.h>

class Model : public Object {
  public:
    Model(std::string path, std::shared_ptr<Shader> shader)
        : path(PROJECT_DIRECTORY + path), Object(shader) {
        this->loadModel();
    };

  private:
    std::string path;
    tinygltf::Model tgModel;
    tinygltf::TinyGLTF tgLoader;
    std::map<unsigned int, Texture2D> textures;

    bool initializedBounds = false;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    void loadModel();

    void processTexture(const tinygltf::Texture &texture, int index);
    void processNode(const tinygltf::Node &node,
                     glm::mat4 parentTransform = {1.0f});
    void processMesh(const tinygltf::Mesh &mesh, glm::mat4 transform);

    void updateBounds(const float *positions, size_t vertexCount);
};
