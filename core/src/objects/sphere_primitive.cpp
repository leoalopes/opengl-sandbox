#include "core/objects/sphere_primitive.hpp"
#include <cmath>
#include <vector>

SpherePrimitive::SpherePrimitive(std::shared_ptr<Shader> shader,
                                 std::shared_ptr<Texture2D> texture,
                                 int latitudeBands, int longitudeBands)
    : Object(shader), texture(texture) {

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

    // Generate vertices
    for (int lat = 0; lat <= latitudeBands; ++lat) {
        float theta = lat * PI / latitudeBands; // 0 to PI
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeBands; ++lon) {
            float phi = lon * 2.0f * PI / longitudeBands; // 0 to 2*PI
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            // Calculate position (unit sphere)
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            // Position
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);

            // Normal (outward-pointing, same as position for unit sphere)
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

            // Texture coordinates
            float u = 1.0f - (float)lon / longitudeBands;
            float v = 1.0f - (float)lat / latitudeBands;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }

    // Generate indices
    for (int lat = 0; lat < latitudeBands; ++lat) {
        for (int lon = 0; lon < longitudeBands; ++lon) {
            int first = lat * (longitudeBands + 1) + lon;
            int second = first + longitudeBands + 1;

            // First triangle (counter-clockwise winding)
            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            // Second triangle (counter-clockwise winding)
            indices.push_back(first + 1);
            indices.push_back(second + 1);
            indices.push_back(second);
        }
    }

    // Create the mesh
    Mesh mainMesh{
        glm::mat4(1.0f),
        positions.data(),
        normals.data(),
        texCoords.data(),
        static_cast<size_t>(
            static_cast<int>(positions.size() / 3)), // vertex count
        indices,
        static_cast<size_t>(static_cast<int>(indices.size())), // index count
        texture.get(),
        nullptr,
        glm::vec2(0.0f),
        glm::vec2(1.0f)};

    this->meshes.push_back(mainMesh);
}
