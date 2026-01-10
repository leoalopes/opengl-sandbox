#pragma once
#include <glad/glad.h>

#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"

#include <vector>

const GLenum DEFAULT_TEXTURE_2D_TYPE = GL_RGBA;
const GLenum DEFAULT_TEXTURE_2D_WRAP_S = GL_REPEAT;
const GLenum DEFAULT_TEXTURE_2D_WRAP_T = GL_REPEAT;
const GLenum DEFAULT_TEXTURE_2D_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
const GLenum DEFAULT_TEXTURE_2D_MAG_FILTER = GL_LINEAR;

class Texture2D : public Texture {
  public:
    Texture2D(std::vector<unsigned char> &textureData, int width, int height,
              GLenum type = DEFAULT_TEXTURE_2D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_2D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_2D_WRAP_T,
              GLenum textureMinFilter = DEFAULT_TEXTURE_2D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_2D_MAG_FILTER);

    Texture2D(unsigned char *textureData, int width, int height,
              GLenum type = DEFAULT_TEXTURE_2D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_2D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_2D_WRAP_T,
              GLenum textureMinFilter = DEFAULT_TEXTURE_2D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_2D_MAG_FILTER);

    Texture2D(std::string path, GLenum type = DEFAULT_TEXTURE_2D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_2D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_2D_WRAP_T,
              GLenum textureMinFilter = DEFAULT_TEXTURE_2D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_2D_MAG_FILTER);

    Texture2D(int width, int height, GLenum type = DEFAULT_TEXTURE_2D_TYPE,
              GLenum textureWrapS = DEFAULT_TEXTURE_2D_WRAP_S,
              GLenum textureWrapT = DEFAULT_TEXTURE_2D_WRAP_T,
              GLenum textureMinFilter = DEFAULT_TEXTURE_2D_MIN_FILTER,
              GLenum textureMagFilter = DEFAULT_TEXTURE_2D_MAG_FILTER);

    void setupTexture(unsigned char *textureData, int width, int height,
                      GLenum type, GLenum textureWrapS, GLenum textureWrapT,
                      GLenum textureMinFilter, GLenum textureMagFilter);

    void bind();
    void unbind();

    void use(int slot, Shader *shader, std::string uniform);
};
