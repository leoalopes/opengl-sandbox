#include <glad/glad.h>

#include "core/graphics/texture_2d.hpp"

#include <stb/stb_image.h>

Texture2D::Texture2D(std::vector<unsigned char> &textureData, int width,
                     int height, GLenum type, GLenum textureWrapS,
                     GLenum textureWrapT, GLenum textureMinFilter,
                     GLenum textureMagFilter)
    : Texture(height, width) {
    this->setupTexture(textureData.data(), width, height, type, textureWrapS,
                       textureWrapT, textureMinFilter, textureMagFilter);
}

Texture2D::Texture2D(unsigned char *textureData, int width, int height,
                     GLenum type, GLenum textureWrapS, GLenum textureWrapT,
                     GLenum textureMinFilter, GLenum textureMagFilter)
    : Texture(height, width) {
    this->setupTexture(textureData, width, height, type, textureWrapS,
                       textureWrapT, textureMinFilter, textureMagFilter);
}

Texture2D::Texture2D(std::string path, GLenum type, GLenum textureWrapS,
                     GLenum textureWrapT, GLenum textureMinFilter,
                     GLenum textureMagFilter)
    : Texture(600, 800) {
    int width, height, nrChannels;
    std::string fullPath{PROJECT_DIRECTORY + path};
    unsigned char *data =
        stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 4);
    this->height = height;
    this->width = width;
    this->setupTexture(data, width, height, type, textureWrapS, textureWrapT,
                       textureMinFilter, textureMagFilter);
    stbi_image_free(data);
}

Texture2D::Texture2D(int width, int height, GLenum type, GLenum textureWrapS,
                     GLenum textureWrapT, GLenum textureMinFilter,
                     GLenum textureMagFilter)
    : Texture(height, width) {
    this->setupTexture(nullptr, width, height, type, textureWrapS, textureWrapT,
                       textureMinFilter, textureMagFilter);
}

void Texture2D::setupTexture(unsigned char *textureData, int width, int height,
                             GLenum type, GLenum textureWrapS,
                             GLenum textureWrapT, GLenum textureMinFilter,
                             GLenum textureMagFilter) {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, this->textureId); };
void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texture2D::use(int slot, Shader *shader, std::string uniform) {
    glActiveTexture(GL_TEXTURE0 + slot);
    this->bind();
    shader->setInt(uniform, slot);
}
