#pragma once

#include "core/graphics/texture.hpp"

class Texture2DMultisample : public Texture {
  public:
    Texture2DMultisample(int width, int height, unsigned int samples);

    void bind();
    void unbind();
};
