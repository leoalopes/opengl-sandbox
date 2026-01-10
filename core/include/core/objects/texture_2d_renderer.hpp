#pragma once

#include "core/graphics/texture_2d.hpp"
#include "core/objects/texture_renderer.hpp"

#include <memory>

class Texture2DRenderer : public TextureRenderer {
  public:
    Texture2DRenderer(unsigned int height, unsigned int width,
                      std::shared_ptr<Texture2D> textureTarget);

    void bind();
    void unbind();
};
