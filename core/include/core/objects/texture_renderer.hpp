#pragma once
#import <glad/glad.h>

#include "core/graphics/texture.hpp"
#include <iostream>
#include <memory>

class TextureRenderer {
  public:
    unsigned int FBO;
    unsigned int RBO;
    std::shared_ptr<Texture> textureTarget;
    int height;
    int width;

    TextureRenderer(int height, int width,
                    std::shared_ptr<Texture> textureTarget)
        : height(height), width(width), textureTarget(textureTarget) {}

    virtual void initialize() {
        glGenFramebuffers(1, &this->FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

        glGenRenderbuffers(1, &this->RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width,
                              this->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, this->RBO);
    }

    virtual void checkStatus() {
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
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"
                          << '\n';
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
    }
};
