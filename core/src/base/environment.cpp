#include "core/base/environment.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

Environment::Environment(std::array<std::string, 6> paths)
    : texture(paths), shader("assets/shaders/environment.vert",
                             "assets/shaders/environment.frag") {
    this->setupBuffer();
}

void Environment::setupBuffer() {
    float skyboxVertices[] = {-1.0f, -1.0f, 1.0f, 3.0f, -1.0f,
                              1.0f,  -1.0f, 3.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
}

void Environment::draw(glm::mat4 view, glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0 + 7);
    this->texture.bind();

    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));

    this->shader.use();
    this->shader.setMatrix("view", glm::value_ptr(viewWithoutTranslation));
    this->shader.setMatrix("projection", glm::value_ptr(projection));
    this->shader.setInt("skybox", 7);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
