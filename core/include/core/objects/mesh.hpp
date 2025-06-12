#pragma once

#include "core/graphics/texture.hpp"
#include "core/objects/object_buffer.hpp"
#include "glm/fwd.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
  public:
    Mesh(const float *positions, const float *normals, const float *texCoords,
         size_t vertexCount, std::vector<unsigned int> indices,
         size_t indexCount, Texture *baseColor, Texture *metallicRoughness,
         glm::vec2 materialOffset, glm::vec2 materialScale);

    Texture *getBaseColor() const { return this->baseColor; };

    Texture *getEmissiveColor() const { return this->emissiveColor; };
    void setEmissiveColor(Texture *emissiveColor) {
        this->emissiveColor = emissiveColor;
    };

    Texture *getNormalTexture() const { return this->normalTexture; };
    void setNormalTexture(Texture *normalTexture) {
        this->normalTexture = normalTexture;
    };

    Texture *getMetallicRoughness() const { return this->metallicRoughness; };

    void draw();

  private:
    ObjectBuffer buffer;
    Texture *baseColor;
    Texture *emissiveColor = nullptr;
    Texture *metallicRoughness;
    Texture *normalTexture = nullptr;

    void mergeVertexAttributes(const float *positions, const float *normals,
                               const float *texCoords, size_t vertexCount,
                               std::vector<unsigned int> indices,
                               glm::vec2 &materialOffset,
                               glm::vec2 &materialScale);
};
