#pragma once

#include "core/graphics/texture.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
  public:
    Mesh(const float *positions, const float *normals, const float *texCoords,
         size_t vertexCount, std::vector<unsigned int> indices,
         size_t indexCount, Texture *baseColor, Texture *metallicRoughness);

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
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture *baseColor;
    Texture *emissiveColor = nullptr;
    Texture *metallicRoughness;
    Texture *normalTexture = nullptr;

    void mergeVertexAttributes(const float *positions, const float *normals,
                               const float *texCoords, size_t vertexCount);

    void setupMesh();
};
