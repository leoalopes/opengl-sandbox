#include "core/objects/texture_2d_renderer_multisample.hpp"
#include "core/graphics/texture_2d_multisample.hpp"

#include <memory>

Texture2DRendererMultisample::Texture2DRendererMultisample(unsigned int height,
                                                           unsigned int width,
                                                           unsigned int samples)
    : TextureRenderer(
          height, width,
          std::make_shared<Texture2DMultisample>(width, height, samples)) {
    this->initialize(samples);

    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->textureTarget->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           this->textureTarget->textureId, 0);

    this->checkStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->blitTexture =
        std::make_shared<Texture2D>(width, height, GL_RGBA, GL_CLAMP_TO_EDGE,
                                    GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
    this->blitTarget =
        std::make_unique<Texture2DRenderer>(height, width, this->blitTexture);
}

void Texture2DRendererMultisample::blit() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->blitTarget->FBO);
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width,
                      this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Texture2DRendererMultisample::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
}
void Texture2DRendererMultisample::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
