#include <glad/glad.h>

#include "core/objects/mesh.hpp"

#include <cstddef>
#include <iostream>

Mesh::Mesh(const float *positions, const float *normals, const float *texCoords,
           size_t vertexCount, std::vector<unsigned int> indices,
           size_t indexCount, Texture *baseColor, Texture *metallicRoughness,
           glm::vec2 materialOffset, glm::vec2 materialScale)
    : buffer({}, {}), baseColor(baseColor),
      metallicRoughness(metallicRoughness) {
    if (vertexCount == 0 || indexCount == 0) {
        std::cerr << "Error: Mesh has no vertices or indices!" << std::endl;
        return;
    }

    this->mergeVertexAttributes(positions, normals, texCoords, vertexCount,
                                indices, materialOffset, materialScale);
}

void Mesh::mergeVertexAttributes(const float *positions, const float *normals,
                                 const float *texCoords, size_t vertexCount,
                                 std::vector<unsigned int> indices,
                                 glm::vec2 &materialOffset,
                                 glm::vec2 &materialScale) {
    std::vector<Vertex> vertices = {};
    vertices.reserve(vertexCount);

    for (size_t i = 0; i < vertexCount; i++) {
        glm::vec3 position{positions[i * 3], positions[i * 3 + 1],
                           positions[i * 3 + 2]};
        glm::vec3 normal{normals[i * 3], normals[i * 3 + 1],
                         normals[i * 3 + 2]};
        glm::vec2 texCoord{(texCoords[i * 2] * materialScale.x) +
                               materialOffset.x,
                           (texCoords[i * 2 + 1] * materialScale.y) +
                               materialOffset.y}; // Fixed stride

        Vertex vertex{position, normal, texCoord};
        vertices.push_back(vertex);
    }

    this->buffer.updateBuffer(vertices, indices);
}

void Mesh::draw() { this->buffer.draw(); }
