#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "gtx/Mesh.h"
#include "gtx/Shaders.h"
#include "gtx/Texture.h"

class ModelLoader
{
    private:

        std::filesystem::path m_modelDirectory;

        std::vector<Mesh> m_meshes;
        Texture m_textures;

        void LoadModel(const std::filesystem::path& path);
        void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
        void ProcessMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);
        void ProcessTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName, std::vector<Mesh::Texture>& textures);

    public:

        ModelLoader(const std::filesystem::path& modelFile);

        void Draw(Shaders& shader);
};
