#pragma once

class Texture {
  public:
    unsigned int textureId;

    int height;
    int width;

    Texture(int height, int width) : height(height), width(width) {};

    virtual void bind() = 0;
    virtual void unbind() = 0;
};
