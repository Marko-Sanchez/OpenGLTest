#include "ModelLoader.h"
#include "assimp/material.h"
#include "glm/gtc/type_ptr.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader(const std::filesystem::path& modelFile)
    :m_modelDirectory(modelFile.parent_path())
{
    this->LoadModel(modelFile);
}

void ModelLoader::Draw(Shaders& shader)
{
    for (auto& mesh: m_meshes)
    {
        mesh.Draw(shader);
    }
}

void ModelLoader::LoadModel(const std::filesystem::path& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
        (
         path,
         aiProcess_Triangulate |
         aiProcess_FlipUVs |
         aiProcess_GenSmoothNormals
        );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    std::cout << "Loaded " << scene->mNumMeshes << " mesh(es) from " << path << std::endl;
    m_meshes.reserve(scene->mNumMeshes);

    this->ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

/*
 * Each node in aiScene only stores its local transform - where it sits relative to it's parent. Therefore,
 * to know where it sits in the world we multiply the node's matrix with it's parent to get the globalTransform.
 */
void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
    // transpose to convert assimps row major to column major (glm/opengl).
    glm::mat4 nodeTransform   = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    for (size_t mIndex {0}; mIndex < node->mNumMeshes; ++mIndex)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[mIndex]];
        this->ProcessMesh(mesh, scene, globalTransform);
    }

    for (size_t cIndex {0}; cIndex < node->mNumChildren; ++cIndex)
    {
        this->ProcessNode(node->mChildren[cIndex], scene, globalTransform);
    }
}

void ModelLoader::ProcessMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4& transform)
{
    // inverse-transposing helps keep normals perpendicular to the model, when 'non-uniform' scaling happens.
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform)));

    // vertex positions, normals, uvs.
    std::vector<Mesh::Vertex> vv;
    vv.reserve(mesh->mNumVertices);

    for (size_t vIndex {0}; vIndex < mesh->mNumVertices; ++vIndex)
    {
        Mesh::Vertex vertex;

        aiVector3D pos = mesh->mVertices[vIndex];
        vertex.position = glm::vec3(transform * glm::vec4{pos.x, pos.y, pos.z, 1.0f});

        // normals have no visual representation, they are direction vector describing which way a surface faces;
        // used for lighting.
        if (mesh->HasNormals())
        {
            aiVector3D normal = mesh->mNormals[vIndex];
            vertex.normal = glm::normalize(normalMatrix * glm::vec3{normal.x, normal.y, normal.z});
        }

        // uvs
        if (mesh->HasTextureCoords(0))
        {
            aiVector3D uv = mesh->mTextureCoords[0][vIndex];
            vertex.texcoords = {uv.x, uv.y};
        }

        vv.emplace_back(vertex);
    }

    // index / ebo buffer.
    std::vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces * 3);

    for (size_t f {0}; f < mesh->mNumFaces; ++f)
    {
        const aiFace& face = mesh->mFaces[f];

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    // process materials.
    std::vector<Mesh::Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // base color of the surface, would be a single color if not added.
    this->ProcessTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    // per-pixel shininess, some areas of a model might be shinier (metals) then others (cloth).
    this->ProcessTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    // this->ProcessTextures(material, aiTextureType_HEIGHT, "texture_normal", textures);

    m_meshes.emplace_back(std::move(vv), std::move(indices), std::move(textures));
}

void ModelLoader::ProcessTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName, std::vector<Mesh::Texture>& textures)
{
    auto textureSize {mat->GetTextureCount(type)};
    for (size_t i {0}; i < textureSize; ++i)
    {
        aiString path;
        mat->GetTexture(type, i, &path);

        std::string spath {path.data};
        auto textureID    {m_textures.UploadTexture(spath, m_modelDirectory / spath)};

        textures.emplace_back(Mesh::Texture{textureID, spath, typeName});
    }
}
