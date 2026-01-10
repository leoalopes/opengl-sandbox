#pragma once

#include "core/base/transform.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture_3d.hpp"
#include "core/objects/mesh.hpp"
#include "core/objects/texture_3d_renderer.hpp"

#include <memory>

class Object {
  public:
    Transform transform;
    std::shared_ptr<Shader> shader;
    std::vector<Mesh> meshes;

    float shininess = 64.0f;
    float borderSize = 0.0f;
    glm::vec3 borderColor;
    bool visible = true;

    glm::vec3 centerPoint = glm::vec3(0.0f);

    bool useDynamicEnvironmentMap = false;
    std::shared_ptr<Texture3D> dynamicEnvironmentMapTextureTarget;
    std::unique_ptr<Texture3DRenderer> dynamicEnvironmentMapRenderer;

    Object(std::shared_ptr<Shader> shader);

    virtual void setupDynamicEnvironmentMap();

    virtual void draw();
    virtual void draw(Shader *overrideShader);
};
