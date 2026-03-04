#pragma once

#include "core/objects/texture_2d_renderer.hpp"
#include "core/objects/texture_renderer.hpp"

class Texture2DRendererMultisample : public TextureRenderer {
  public:
    std::shared_ptr<Texture2D> blitTexture;
    std::unique_ptr<Texture2DRenderer> blitTarget;

    Texture2DRendererMultisample(unsigned int height, unsigned int width,
                                 unsigned int samples);

    void blit();
    void bind();
    void unbind();
};
