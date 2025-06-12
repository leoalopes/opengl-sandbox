#include "core/base/common.hpp"

#include <stb/stb_image.h>
#include <vector>

std::vector<Vertex> vertices{
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
};

ObjectBuffer quadBuffer{vertices, {0, 1, 3, 1, 2, 3}};

// Texture lightbulbTexture{"assets/texture/lightbulb.png"};

/*Shader unlintTexturedShader{"assets/shaders/standard.vert",*/
/*                            "assets/shaders/unlit-textured.frag"};*/
/**/
/*Shader unlintSolidColorShader{"assets/shaders/standard.vert",*/
/*                              "assets/shaders/unlit-solid.frag"};*/
