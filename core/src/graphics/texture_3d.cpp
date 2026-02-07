#include "core/graphics/texture_3d.hpp"

#include <iostream>
#include <stb/stb_image.h>

Texture3D::Texture3D(std::array<std::string, 6> paths, GLenum type,
                     GLenum textureWrapS, GLenum textureWrapT,
                     GLenum textureWrapR, GLenum textureMinFilter,
                     GLenum textureMagFilter)
    : Texture(800, 800) {
    this->setupTexture(textureWrapS, textureWrapT, textureWrapR,
                       textureMinFilter, textureMagFilter);

    for (int i = 0; i < paths.size(); i++) {
        this->loadTextureFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, paths[i],
                              type);
    }
};

Texture3D::Texture3D(int width, int height, GLenum type, GLenum textureWrapS,
                     GLenum textureWrapT, GLenum textureWrapR,
                     GLenum textureMinFilter, GLenum textureMagFilter)
    : Texture(height, width) {
    this->setupTexture(textureWrapS, textureWrapT, textureWrapR,
                       textureMinFilter, textureMagFilter);

    for (int i = 0; i < 6; i++) {
        this->setupTextureFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, type, width,
                               height, NULL);
    }
};

void Texture3D::setupTexture(GLenum textureWrapS, GLenum textureWrapT,
                             GLenum textureWrapR, GLenum textureMinFilter,
                             GLenum textureMagFilter) {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
};

void Texture3D::loadTextureFace(GLenum face, std::string path, GLenum type) {
    std::string filePath = PROJECT_DIRECTORY + path;
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        this->height = height;
        this->width = width;
        this->setupTextureFace(face, type, width, height, data);
    } else {
        std::cout << "Failed to load face texture" << '\n';
    }
    stbi_image_free(data);
};

void Texture3D::setupTextureFace(GLenum face, GLenum type, int width,
                                 int height, unsigned char *data) {
    glTexImage2D(face, 0, type, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
};

void Texture3D::bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId); };

void Texture3D::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); };
