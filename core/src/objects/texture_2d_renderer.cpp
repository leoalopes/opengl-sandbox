#include "core/objects/texture_2d_renderer.hpp"
#include "core/graphics/texture_2d.hpp"

#include <memory>

Texture2DRenderer::Texture2DRenderer(unsigned int height, unsigned int width,
                                     std::shared_ptr<Texture2D> textureTarget)
    : TextureRenderer(height, width, textureTarget) {
    this->initialize();

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->textureTarget->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->textureTarget->textureId, 0);

    this->checkStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture2DRenderer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, this->FBO); }
void Texture2DRenderer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
