#include "core/objects/cube_primitive.hpp"

CubePrimitive::CubePrimitive(std::shared_ptr<Shader> shader,
                             std::shared_ptr<Texture2D> texture)
    : Object(shader), texture(texture) {
    static const float positions[] = {
        // Front face (facing +Z) - CCW when viewed from outside
        -1.0f, -1.0f, 1.0f, // Bottom-left
        1.0f, -1.0f, 1.0f,  // Bottom-right
        1.0f, 1.0f, 1.0f,   // Top-right
        -1.0f, 1.0f, 1.0f,  // Top-left

        // Back face (facing -Z) - CCW when viewed from outside
        1.0f, -1.0f, -1.0f,  // Bottom-left
        -1.0f, -1.0f, -1.0f, // Bottom-right
        -1.0f, 1.0f, -1.0f,  // Top-right
        1.0f, 1.0f, -1.0f,   // Top-left

        // Left face (facing -X) - CCW when viewed from outside
        -1.0f, -1.0f, -1.0f, // Bottom-left
        -1.0f, -1.0f, 1.0f,  // Bottom-right
        -1.0f, 1.0f, 1.0f,   // Top-right
        -1.0f, 1.0f, -1.0f,  // Top-left

        // Right face (facing +X) - CCW when viewed from outside
        1.0f, -1.0f, 1.0f,  // Bottom-left
        1.0f, -1.0f, -1.0f, // Bottom-right
        1.0f, 1.0f, -1.0f,  // Top-right
        1.0f, 1.0f, 1.0f,   // Top-left

        // Bottom face (facing -Y) - CCW when viewed from outside
        -1.0f, -1.0f, -1.0f, // Bottom-left
        1.0f, -1.0f, -1.0f,  // Bottom-right
        1.0f, -1.0f, 1.0f,   // Top-right
        -1.0f, -1.0f, 1.0f,  // Top-left

        // Top face (facing +Y) - CCW when viewed from outside
        -1.0f, 1.0f, 1.0f, // Bottom-left
        1.0f, 1.0f, 1.0f,  // Bottom-right
        1.0f, 1.0f, -1.0f, // Top-right
        -1.0f, 1.0f, -1.0f // Top-left
    };

    static const float normals[] = {
        // Front face
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // Back face
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,

        // Left face
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f,

        // Right face
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.0f,

        // Top face
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    static const float texCoords[] = {
        // Front face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f, // Top-left

        // Back face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f, // Top-left

        // Left face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f, // Top-left

        // Right face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f, // Top-left

        // Bottom face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f, // Top-left

        // Top face
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f  // Top-left
    };

    std::vector<unsigned int> indices = {// Front face
                                         0, 1, 2, 2, 3, 0,
                                         // Back face
                                         4, 5, 6, 6, 7, 4,
                                         // Left face
                                         8, 9, 10, 10, 11, 8,
                                         // Right face
                                         12, 13, 14, 14, 15, 12,
                                         // Bottom face
                                         16, 17, 18, 18, 19, 16,
                                         // Top face
                                         20, 21, 22, 22, 23, 20};

    Mesh mainMesh{
        glm::mat4(1.0f), positions, normals,       texCoords, 24,
        indices,         36,        texture.get(), nullptr,   glm::vec2(0.0f),
        glm::vec2(1.0f)};

    this->meshes.push_back(mainMesh);
}
