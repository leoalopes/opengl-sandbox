#include "core/objects/texture_renderer.hpp"
#include "core/graphics/texture.hpp"
#include <iostream>

TextureRenderer::TextureRenderer(int height, int width)
    : height(height), width(width), texture(NULL, 1280, 720, GL_RGBA, GL_REPEAT,
                                            GL_REPEAT, GL_LINEAR, GL_LINEAR) {
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    this->texture.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           this->texture.textureId, 0);

    glGenRenderbuffers(1, &this->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width,
                          this->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, this->RBO);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Error: Framebuffer is not complete!" << '\n';
        if (status == GL_FRAMEBUFFER_UNDEFINED) {
            std::cout << "GL_FRAMEBUFFER_UNDEFINED" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << '\n';
        }
        if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
            std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << '\n';
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS) {
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << '\n';
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureRenderer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, this->FBO); }
void TextureRenderer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
