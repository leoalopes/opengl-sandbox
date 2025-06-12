#include <glad/glad.h>

#include "core/graphics/texture.hpp"

#include <stb/stb_image.h>

Texture::Texture(std::vector<unsigned char> &textureData, int width, int height,
                 GLenum type, GLenum textureWrapS, GLenum textureWrapT,
                 GLenum textureMinFilter, GLenum textureMagFilter) {
    this->setupTexture(textureData.data(), width, height, type, textureWrapS,
                       textureWrapT, textureMinFilter, textureMagFilter);
}

Texture::Texture(unsigned char *textureData, int width, int height, GLenum type,
                 GLenum textureWrapS, GLenum textureWrapT,
                 GLenum textureMinFilter, GLenum textureMagFilter) {
    this->setupTexture(textureData, width, height, type, textureWrapS,
                       textureWrapT, textureMinFilter, textureMagFilter);
}

Texture::Texture(std::string path, GLenum type, GLenum textureWrapS,
                 GLenum textureWrapT, GLenum textureMinFilter,
                 GLenum textureMagFilter) {
    int width, height, nrChannels;
    std::string fullPath{PROJECT_DIRECTORY + path};
    unsigned char *data =
        stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 4);
    this->setupTexture(data, width, height, type, textureWrapS, textureWrapT,
                       textureMinFilter, textureMagFilter);
    stbi_image_free(data);
}

void Texture::setupTexture(unsigned char *textureData, int width, int height,
                           GLenum type, GLenum textureWrapS,
                           GLenum textureWrapT, GLenum textureMinFilter,
                           GLenum textureMagFilter) {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type,
                 GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, this->textureId); };
void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texture::use(int slot, Shader *shader, std::string uniform) {
    glActiveTexture(GL_TEXTURE0 + slot);
    this->bind();
    shader->setInt(uniform, slot);
}
