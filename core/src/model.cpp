#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include <core/model.hpp>

#include <iostream>

void Model::loadModel() {
    std::string error;
    std::string warn;
    bool loaded = tgLoader.LoadASCIIFromFile(&tgModel, &error, &warn, path);

    if (!warn.empty()) {
        std::cout << "Warning loading .gltf file: " << warn << '\n';
    }

    if (!error.empty()) {
        std::cout << "Error loading .gltf file: " << error << '\n';
    }

    if (!loaded) {
        std::cout << "Failed to parse .gltf file" << '\n';
        throw "Failed to load model from path " + path;
    }

    for (size_t i = 0; i < tgModel.textures.size(); i++) {
        this->processTexture(tgModel.textures.at(i), i);
    }

    tinygltf::Scene &rootScene = tgModel.scenes[tgModel.defaultScene];
    for (int nodeIndex : rootScene.nodes) {
        this->processNode(tgModel.nodes[nodeIndex]);
    }
}

void Model::processTexture(const tinygltf::Texture &texture, int index) {
    if (texture.source < 0) {
        std::cerr << "Invalid image index in texture" << std::endl;
        return;
    }

    std::vector<unsigned char> image = tgModel.images[texture.source].image;
    int width = tgModel.images[texture.source].width;
    int height = tgModel.images[texture.source].height;
    this->textures.insert({index, {image, width, height}});
}

void Model::processNode(const tinygltf::Node &node) {
    if (node.mesh >= 0) {
        const tinygltf::Mesh &mesh = tgModel.meshes[node.mesh];
        this->processMesh(mesh);
    }

    for (int childIndex : node.children) {
        this->processNode(tgModel.nodes[childIndex]);
    }
}

void Model::processMesh(const tinygltf::Mesh &mesh) {
    for (const tinygltf::Primitive &primitive : mesh.primitives) {
        if (primitive.mode != 4) {
            std::cerr << "Error: skipping primitive due a non-triangle mode "
                      << primitive.mode << "!" << '\n';
            continue;
        }

        const auto &attributes = primitive.attributes;
        if (attributes.find("POSITION") == attributes.end()) {
            std::cerr << "Error: skipping primitive due to missing positions!"
                      << '\n';
            continue;
        }
        if (attributes.find("NORMAL") == attributes.end()) {
            std::cerr << "Error: skipping primitive due to missing positions!"
                      << '\n';
            continue;
        }
        if (attributes.find("TEXCOORD_0") == attributes.end()) {
            std::cerr << "Error: skipping primitive due to missing positions!"
                      << '\n';
            continue;
        }
        if (primitive.indices < 0) {
            std::cerr << "Error: skipping primitive due to missing indices!"
                      << '\n';
            continue;
        }
        if (primitive.material < 0) {
            std::cerr << "Error: skipping primitive due to missing material!"
                      << '\n';
            continue;
        }

        const tinygltf::Accessor &positionAccessor =
            tgModel.accessors[attributes.at("POSITION")];
        const tinygltf::BufferView &positionBufferView =
            tgModel.bufferViews[positionAccessor.bufferView];
        const tinygltf::Buffer &positionBuffer =
            tgModel.buffers[positionBufferView.buffer];
        const float *positions = reinterpret_cast<const float *>(
            &positionBuffer.data[positionBufferView.byteOffset +
                                 positionAccessor.byteOffset]);

        const tinygltf::Accessor &normalAccessor =
            tgModel.accessors[attributes.at("NORMAL")];
        const tinygltf::BufferView &normalBufferView =
            tgModel.bufferViews[normalAccessor.bufferView];
        const tinygltf::Buffer &normalBuffer =
            tgModel.buffers[normalBufferView.buffer];
        const float *normals = reinterpret_cast<const float *>(
            &normalBuffer.data[normalBufferView.byteOffset +
                               normalAccessor.byteOffset]);

        const tinygltf::Accessor &texAccessor =
            tgModel.accessors[attributes.at("TEXCOORD_0")];
        const tinygltf::BufferView &texBufferView =
            tgModel.bufferViews[texAccessor.bufferView];
        const tinygltf::Buffer &texBuffer =
            tgModel.buffers[texBufferView.buffer];
        const float *texCoords = reinterpret_cast<const float *>(
            &texBuffer.data[texBufferView.byteOffset + texAccessor.byteOffset]);

        std::vector<unsigned int> convertedIndices;
        const tinygltf::Accessor &indexAccessor =
            tgModel.accessors[primitive.indices];
        const tinygltf::BufferView &indexBufferView =
            tgModel.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer &indexBuffer =
            tgModel.buffers[indexBufferView.buffer];

        const unsigned char *indexData = indexBuffer.data.data() +
                                         indexBufferView.byteOffset +
                                         indexAccessor.byteOffset;
        switch (indexAccessor.componentType) {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                const unsigned char *indices =
                    reinterpret_cast<const unsigned char *>(indexData);
                convertedIndices.reserve(indexAccessor.count);
                for (size_t i = 0; i < indexAccessor.count; i++) {
                    convertedIndices.push_back(
                        static_cast<unsigned int>(indices[i]));
                }
                break;
            }
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                const unsigned short *indices =
                    reinterpret_cast<const unsigned short *>(indexData);
                convertedIndices.reserve(indexAccessor.count);
                for (size_t i = 0; i < indexAccessor.count; i++) {
                    convertedIndices.push_back(
                        static_cast<unsigned int>(indices[i]));
                }
                break;
            }
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                const unsigned int *indices =
                    reinterpret_cast<const unsigned int *>(indexData);
                convertedIndices.assign(indices, indices + indexAccessor.count);
                break;
            }
            default:
                std::cerr << "Error: Unsupported index component type: "
                          << indexAccessor.componentType << '\n';
                continue;
        }

        if (positionAccessor.count != normalAccessor.count ||
            positionAccessor.count != texAccessor.count ||
            indexAccessor.count <= 0) {
            std::cerr << "Error: skipping primitive due mismatch between "
                         "count of accessors!"
                      << '\n';
            std::cout << "Positions: " << positionAccessor.count << '\n';
            std::cout << "Normals: " << normalAccessor.count << '\n';
            std::cout << "Textures: " << texAccessor.count << '\n';
            std::cout << "Indices: " << indexAccessor.count << '\n';
            continue;
        }

        tinygltf::Material material = tgModel.materials[primitive.material];
        const int baseColorTexture =
            material.pbrMetallicRoughness.baseColorTexture.index;
        const int metallicRoughnessTexture =
            material.pbrMetallicRoughness.metallicRoughnessTexture.index;

        if (baseColorTexture < 0) {
            std::cerr
                << "Error: skipping primitive due to missing baseColorTexture!"
                << '\n';
            continue;
        }
        if (metallicRoughnessTexture < 0) {
            std::cerr << "Error: skipping primitive due to missing "
                         "metallicRoughnessTexture!"
                      << '\n';
            continue;
        }

        Mesh meshInstance{positions,
                          normals,
                          texCoords,
                          positionAccessor.count,
                          convertedIndices,
                          indexAccessor.count,
                          &this->textures.at(baseColorTexture),
                          &this->textures.at(metallicRoughnessTexture)};

        const int emissiveTexture = material.emissiveTexture.index;
        if (emissiveTexture >= 0) {
            meshInstance.setEmissiveColor(&this->textures.at(emissiveTexture));
        }

        const int normalTexture = material.normalTexture.index;
        if (normalTexture >= 0) {
            meshInstance.setNormalTexture(&this->textures.at(normalTexture));
        }

        this->meshes.push_back(meshInstance);
        std::cout << "Added mesh " << mesh.name << '\n';
    }
}

void Model::draw() { this->draw(this->shader.get()); }

void Model::draw(Shader *overrideShader) {
    glm::mat4 modelMatrix = this->transform.getMatrix();
    glm::mat4 normalMatrix = glm::inverse(glm::transpose(modelMatrix));
    overrideShader->setMatrix("model", glm::value_ptr(modelMatrix));
    overrideShader->setMatrix("normalMatrix", glm::value_ptr(modelMatrix));

    for (unsigned int i = 0; i < this->meshes.size(); i++) {
        Mesh mesh = this->meshes[i];

        mesh.getBaseColor()->use(0, overrideShader, "material.baseColor");
        mesh.getMetallicRoughness()->use(1, overrideShader,
                                         "material.metallicRoughness");

        Texture *emissiveTexture = mesh.getEmissiveColor();
        if (emissiveTexture != nullptr) {
            emissiveTexture->use(2, overrideShader, "material.emissiveColor");
        }
        overrideShader->setInt("material.hasEmissiveColor",
                               emissiveTexture != nullptr);

        Texture *normalTexture = mesh.getNormalTexture();
        if (normalTexture != nullptr) {
            normalTexture->use(3, overrideShader, "material.normalTexture");
        }
        overrideShader->setInt("material.hasNormalTexture",
                               normalTexture != nullptr);

        overrideShader->setFloat("material.shininess", 64.0f);

        mesh.draw();
    }
}
