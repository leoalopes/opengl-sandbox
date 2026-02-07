#pragma once
#include <glad/glad.h>

#include "core/graphics/texture.hpp"

#include <array>
#include <string>

const GLenum DEFAULT_TEXTURE_3D_TYPE = GL_RGBA;
const GLenum DEFAULT_TEXTURE_3D_WRAP_S = GL_CLAMP_TO_EDGE;
const GLenum DEFAULT_TEXTURE_3D_WRAP_T = GL_CLAMP_TO_EDGE;
const GLenum DEFAULT_TEXTURE_3D_WRAP_R = GL_CLAMP_TO_EDGE;
const GLenum DEFAULT_TEXTURE_3D_MIN_FILTER = GL_LINEAR;
const GLenum DEFAULT_TEXTURE_3D_MAG_FILTER = GL_LINEAR;

class Texture3D : public Texture {
  public:
    Texture3D(std::array<std::string, 6> paths,
              GLenum type = DEFAULT_TEXTURE_3D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_3D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_3D_WRAP_T,
              GLenum textureWrapR = DEFAULT_TEXTURE_3D_WRAP_R,
              GLenum textureMinFilter = DEFAULT_TEXTURE_3D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_3D_MAG_FILTER);

    Texture3D(int width, int height, GLenum type = DEFAULT_TEXTURE_3D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_3D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_3D_WRAP_T,
              GLenum textureWrapR = DEFAULT_TEXTURE_3D_WRAP_R,
              GLenum textureMinFilter = DEFAULT_TEXTURE_3D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_3D_MAG_FILTER);

    void setupTexture(GLenum textureWrapS, GLenum textureWrapT,
                      GLenum textureWrapR, GLenum textureMinFilter,
                      GLenum textureMagFilter);
    void loadTextureFace(GLenum face, std::string facePath, GLenum type);
    void setupTextureFace(GLenum face, GLenum type, int width, int height,
                          unsigned char *data);

    void bind();
    void unbind();
};
