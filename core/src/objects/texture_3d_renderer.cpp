#include "core/objects/texture_3d_renderer.hpp"
#include "core/graphics/texture_3d.hpp"

#include <iostream>
#include <memory>

Texture3DRenderer::Texture3DRenderer(unsigned int height, unsigned int width,
                                     std::shared_ptr<Texture3D> textureTarget)
    : TextureRenderer(height, width, textureTarget) {
    if (this->textureTarget.get() == nullptr) {
        std::cout << "Warning: skipping initializing Texture3DRenderer due to "
                     "missing target!"
                  << '\n';
        return;
    }

    this->initialize();

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->textureTarget->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                           this->textureTarget->textureId, 0);

    this->checkStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture3DRenderer::bind(unsigned int face) {
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                           this->textureTarget->textureId, 0);
}
void Texture3DRenderer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
