#include "core/objects/object.hpp"

#include "core/objects/texture_3d_renderer.hpp"

#include <memory>

Object::Object(std::shared_ptr<Shader> shader) : shader(shader) {}

void Object::setupDynamicEnvironmentMap() {
    this->useDynamicEnvironmentMap = true;
    this->dynamicEnvironmentMapTextureTarget =
        std::make_shared<Texture3D>(800, 800);
    this->dynamicEnvironmentMapRenderer = std::make_unique<Texture3DRenderer>(
        800, 800, this->dynamicEnvironmentMapTextureTarget);
}

void Object::draw() { this->draw(this->shader.get()); }

void Object::draw(Shader *overrideShader) {
    glm::mat4 modelMatrix = this->transform.getMatrix(-this->centerPoint);
    for (unsigned int i = 0; i < this->meshes.size(); i++) {
        Mesh mesh = this->meshes[i];

        if (mesh.getBaseColor() != nullptr) {
            mesh.getBaseColor()->use(0, overrideShader, "material.baseColor");
        }

        if (mesh.getMetallicRoughness() != nullptr) {
            mesh.getMetallicRoughness()->use(1, overrideShader,
                                             "material.metallicRoughness");
        }

        Texture2D *emissiveTexture = mesh.getEmissiveColor();
        if (emissiveTexture != nullptr) {
            emissiveTexture->use(2, overrideShader, "material.emissiveColor");
        }
        overrideShader->setInt("material.hasEmissiveColor",
                               emissiveTexture != nullptr);

        Texture2D *normalTexture = mesh.getNormalTexture();
        if (normalTexture != nullptr) {
            normalTexture->use(3, overrideShader, "material.normalTexture");
        }
        overrideShader->setInt("material.hasNormalTexture",
                               normalTexture != nullptr);

        overrideShader->setFloat("material.shininess", this->shininess);

        mesh.draw(overrideShader, modelMatrix);
    }
}
