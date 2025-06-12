#include "core/base/environment.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb/stb_image.h>

Environment::Environment(std::string basePath)
    : basePath(PROJECT_DIRECTORY + basePath),
      shader("assets/shaders/environment.vert",
             "assets/shaders/environment.frag") {
    this->setupBuffer();
    this->setupTexture();
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

void Environment::setupTexture() {
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);

    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "front.jpg");
    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "back.jpg");

    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, "right.jpg");
    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "left.jpg");

    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "top.jpg");
    this->setupTextureFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "bottom.jpg");

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Environment::setupTextureFace(GLenum face, std::string facePath) {
    std::string filePath = this->basePath + "/" + facePath;
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(face, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load skybox face texture" << '\n';
    }
    stbi_image_free(data);
}

void Environment::draw(glm::mat4 view, glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);

    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));

    this->shader.use();
    this->shader.setMatrix("view", glm::value_ptr(viewWithoutTranslation));
    this->shader.setMatrix("projection", glm::value_ptr(projection));
    this->shader.setInt("skybox", 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
