#pragma once

#include "core/graphics/texture_3d.hpp"
#include "core/objects/texture_renderer.hpp"

class Texture3DRenderer : public TextureRenderer {
  public:
    Texture3DRenderer(unsigned int height, unsigned int width,
                      std::shared_ptr<Texture3D> textureTarget);

    void bind(unsigned int face);
    void unbind();
};
