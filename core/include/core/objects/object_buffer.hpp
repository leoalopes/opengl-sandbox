#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class ObjectBuffer {
  public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    ObjectBuffer(std::vector<Vertex> vertices,
                 std::vector<unsigned int> indices);

    void updateBuffer(std::vector<Vertex> vertices,
                      std::vector<unsigned int> indices);

    void draw();
};
