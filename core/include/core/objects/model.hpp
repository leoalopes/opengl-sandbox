#pragma once

#include "core/graphics/shader.hpp"
#include "core/objects/object.hpp"
#include "glm/fwd.hpp"

#include <memory>
#include <string>
#include <tiny_gltf/tiny_gltf.h>

class Model : public Object {
  public:
    std::map<unsigned int, Texture2D> textures;

    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    Model(std::string path, std::shared_ptr<Shader> shader)
        : path(PROJECT_DIRECTORY + path), Object(shader) {
        this->loadModel();
    };

    Model(Model *modelToCopy)
        : path(modelToCopy->path), Object(modelToCopy->shader) {
        this->meshes = modelToCopy->meshes;
        this->textures = modelToCopy->textures;
        this->centerPoint = modelToCopy->centerPoint;
        this->minBounds = modelToCopy->minBounds;
        this->maxBounds = modelToCopy->maxBounds;
        this->initializedBounds = true;
    }

  private:
    std::string path;
    tinygltf::Model tgModel;
    tinygltf::TinyGLTF tgLoader;

    bool initializedBounds = false;

    void loadModel();

    void processTexture(const tinygltf::Texture &texture, int index);
    void processNode(const tinygltf::Node &node,
                     glm::mat4 parentTransform = {1.0f});
    void processMesh(const tinygltf::Mesh &mesh, glm::mat4 transform);

    void updateBounds(const float *positions, size_t vertexCount);
};
