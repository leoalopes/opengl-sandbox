#pragma once
#include <glad/glad.h>

class PointBuffer {
  public:
    unsigned int VAO;

    PointBuffer();

    void draw();
};
