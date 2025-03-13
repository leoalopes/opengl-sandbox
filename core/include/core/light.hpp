#pragma once

#include "core/shader.hpp"
#include <glm/glm.hpp>

class Light {
  public:
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 diffuse, glm::vec3 specular)
        : diffuse(diffuse), specular(specular) {}

    virtual void updateShader(std::string prefix, Shader *shader) {
        shader->use();
        shader->setVector(prefix + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
        shader->setVector(prefix + ".specular", specular.x, specular.y,
                          specular.z);
    }
};

class DirectionalLight : public Light {
  public:
    glm::vec3 ambient;
    glm::vec3 direction;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                     glm::vec3 specular)
        : Light(diffuse, specular), ambient(ambient), direction(direction) {}

    void updateDirection(glm::vec3 direction) { this->direction = direction; }

    void updateShader(std::string prefix, Shader *shader) override {
        Light::updateShader(prefix, shader);
        shader->setVector(prefix + ".ambient", ambient.x, ambient.y, ambient.z);
        shader->setVector(prefix + ".direction", direction.x, direction.y,
                          direction.z);
    }
};

class PointLight : public Light {
  public:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular,
               float constant, float linear, float quadratic)
        : Light(diffuse, specular), position(position), constant(constant),
          linear(linear), quadratic(quadratic) {}

    void updatePosition(glm::vec3 position) { this->position = position; }

    void updateAttenuation(float constant, float linear, float quadratic) {
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }

    void updateShader(std::string prefix, Shader *shader) override {
        Light::updateShader(prefix, shader);
        shader->setVector(prefix + ".position", position.x, position.y,
                          position.z);

        shader->setFloat(prefix + ".constant", constant);
        shader->setFloat(prefix + ".linear", linear);
        shader->setFloat(prefix + ".quadratic", quadratic);
    }
};

class SpotLight : public Light {
  public:
    glm::vec3 position;
    glm::vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff,
              float outerCutOff, glm::vec3 diffuse, glm::vec3 specular,
              float constant, float linear, float quadratic)
        : Light(diffuse, specular), position(position), direction(direction),
          cutOff(cutOff), outerCutOff(outerCutOff), constant(constant),
          linear(linear), quadratic(quadratic) {}

    void updatePosition(glm::vec3 position) { this->position = position; }
    void updateDirection(glm::vec3 direction) { this->direction = direction; }

    void updateAttenuation(float constant, float linear, float quadratic) {
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }

    void updateCutoff(float cutOff, float outerCutOff) {
        this->cutOff = cutOff;
        this->outerCutOff = outerCutOff;
    }

    void updateShader(std::string prefix, Shader *shader) override {
        Light::updateShader(prefix, shader);
        shader->setVector(prefix + ".position", position.x, position.y,
                          position.z);
        shader->setVector(prefix + ".direction", direction.x, direction.y,
                          direction.z);

        shader->setFloat(prefix + ".cutOff", cutOff);
        shader->setFloat(prefix + ".outerCutOff", outerCutOff);

        shader->setFloat(prefix + ".constant", constant);
        shader->setFloat(prefix + ".linear", linear);
        shader->setFloat(prefix + ".quadratic", quadratic);
    }
};
