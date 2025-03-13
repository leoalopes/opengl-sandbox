#pragma once
#include "core/shader.hpp"
#include <glad/glad.h>
#include <vector>

const GLenum DEFAULT_TEXTURE_TYPE = GL_RGBA;
const GLenum DEFAULT_TEXTURE_WRAP_S = GL_REPEAT;
const GLenum DEFAULT_TEXTURE_WRAP_T = GL_REPEAT;
const GLenum DEFAULT_TEXTURE_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
const GLenum DEFAULT_TEXTURE_MAG_FILTER = GL_LINEAR;

class Texture {
  public:
    Texture(std::vector<unsigned char> &textureData, int width, int height,
            GLenum type = DEFAULT_TEXTURE_TYPE,
            GLenum textureWrapS = DEFAULT_TEXTURE_WRAP_S,
            GLenum textureWrapT = DEFAULT_TEXTURE_WRAP_T,
            GLenum textureMinFilter = DEFAULT_TEXTURE_MIN_FILTER,
            GLenum textureMagFilter = DEFAULT_TEXTURE_MAG_FILTER);

    void bind();
    void unbind();

    void use(int slot, Shader *shader, std::string uniform);

  private:
    unsigned int textureId;
};
