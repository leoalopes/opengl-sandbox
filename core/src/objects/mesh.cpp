#include <glad/glad.h>

#include "core/objects/mesh.hpp"

#include <cstddef>
#include <iostream>

Mesh::Mesh(const float *positions, const float *normals, const float *texCoords,
           size_t vertexCount, std::vector<unsigned int> indices,
           size_t indexCount, Texture *baseColor, Texture *metallicRoughness)
    : indices(indices), baseColor(baseColor),
      metallicRoughness(metallicRoughness) {
    if (vertexCount == 0 || indexCount == 0) {
        std::cerr << "Error: Mesh has no vertices or indices!" << std::endl;
        return;
    }

    this->mergeVertexAttributes(positions, normals, texCoords, vertexCount);
    this->setupMesh();
}

void Mesh::mergeVertexAttributes(const float *positions, const float *normals,
                                 const float *texCoords, size_t vertexCount) {
    for (size_t i = 0; i < vertexCount; i++) {
        glm::vec3 position{positions[i * 3], positions[i * 3 + 1],
                           positions[i * 3 + 2]};
        glm::vec3 normal{normals[i * 3], normals[i * 3 + 1],
                         normals[i * 3 + 2]};
        glm::vec2 texCoord{texCoords[i * 2],
                           texCoords[i * 2 + 1]}; // Fixed stride

        Vertex vertex{position, normal, texCoord};
        this->vertices.push_back(vertex);
    }
}

void Mesh::setupMesh() {
    if (vertices.empty() || indices.empty()) {
        std::cerr << "Error: Cannot set up mesh with empty data!" << std::endl;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw() {
    if (indices.empty()) {
        std::cerr << "Error: No indices to draw!" << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
