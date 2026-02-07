#include "core/objects/point_buffer.hpp"

PointBuffer::PointBuffer() { glGenVertexArrays(1, &this->VAO); }

void PointBuffer::draw() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}
