#pragma once
#include "core/graphics/texture.hpp"

class TextureRenderer {
  public:
    unsigned int FBO;
    unsigned int RBO;
    Texture texture;
    int height;
    int width;

    TextureRenderer(int height, int width);

    void bind();
    void unbind();
};
