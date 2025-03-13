#include <glad/glad.h>

#include "core/texture.hpp"
#include <stb/stb_image.h>

Texture::Texture(std::vector<unsigned char> &textureData, int width, int height,
                 GLenum type, GLenum textureWrapS, GLenum textureWrapT,
                 GLenum textureMinFilter, GLenum textureMagFilter) {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type,
                 GL_UNSIGNED_BYTE, textureData.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, this->textureId); };
void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texture::use(int slot, Shader *shader, std::string uniform) {
    glActiveTexture(GL_TEXTURE0 + slot);
    this->bind();
    shader->setInt(uniform, slot);
}
