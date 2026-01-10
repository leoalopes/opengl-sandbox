#pragma once

#include "core/graphics/texture_2d.hpp"
#include "core/objects/object_buffer.hpp"
#include "glm/fwd.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
  public:
    Mesh(glm::mat4 transform, const float *positions, const float *normals,
         const float *texCoords, size_t vertexCount,
         std::vector<unsigned int> indices, size_t indexCount,
         Texture2D *baseColor, Texture2D *metallicRoughness,
         glm::vec2 materialOffset, glm::vec2 materialScale);

    Texture2D *getBaseColor() const { return this->baseColor; };

    Texture2D *getEmissiveColor() const { return this->emissiveColor; };
    void setEmissiveColor(Texture2D *emissiveColor) {
        this->emissiveColor = emissiveColor;
    };

    Texture2D *getNormalTexture() const { return this->normalTexture; };
    void setNormalTexture(Texture2D *normalTexture) {
        this->normalTexture = normalTexture;
    };

    Texture2D *getMetallicRoughness() const { return this->metallicRoughness; };

    void draw(Shader *shader, glm::mat4 parentTransform);

  private:
    ObjectBuffer buffer;
    Texture2D *baseColor;
    Texture2D *emissiveColor = nullptr;
    Texture2D *metallicRoughness;
    Texture2D *normalTexture = nullptr;
    glm::mat4 transform;

    void mergeVertexAttributes(const float *positions, const float *normals,
                               const float *texCoords, size_t vertexCount,
                               std::vector<unsigned int> indices,
                               glm::vec2 &materialOffset,
                               glm::vec2 &materialScale);
};
