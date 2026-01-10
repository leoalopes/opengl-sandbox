#include "core/objects/quad_primitive.hpp"

QuadPrimitive::QuadPrimitive(std::shared_ptr<Shader> shader,
                             std::shared_ptr<Texture2D> texture)
    : Object(shader), texture(texture) {
    static const float positions[] = {
        -1.0f, -1.0f, 0.0f, // Bottom-left
        1.0f,  -1.0f, 0.0f, // Bottom-right
        1.0f,  1.0f,  0.0f, // Top-right
        -1.0f, 1.0f,  0.0f  // Top-left
    };

    static const float normals[] = {
        0.0f, 0.0f, 1.0f, // Bottom-left
        0.0f, 0.0f, 1.0f, // Bottom-right
        0.0f, 0.0f, 1.0f, // Top-right
        0.0f, 0.0f, 1.0f  // Top-left
    };

    static const float texCoords[] = {
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f  // Top-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle (bottom-left, bottom-right, top-right)
        2, 3, 0  // Second triangle (top-right, top-left, bottom-left)
    };

    Mesh mainMesh{
        glm::mat4(1.0f), positions, normals,       texCoords, 4,
        indices,         6,         texture.get(), nullptr,   glm::vec2(0.0f),
        glm::vec2(1.0f)};

    this->meshes.push_back(mainMesh);
}
