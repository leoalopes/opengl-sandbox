#include <glad/glad.h>

#include "core/graphics/texture_2d_multisample.hpp"

Texture2DMultisample::Texture2DMultisample(int width, int height,
                                           unsigned int samples)
    : Texture(height, width) {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F,
                            width, height, GL_TRUE);
}

void Texture2DMultisample::bind() {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->textureId);
};

void Texture2DMultisample::unbind() {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
};
