#include "Model3D.h"

#include "3D/Material.h"
#include "3D/Mesh3D.h"
#include "3D/Model3D.h"
#include "3D/VertexData.h"
#include "Renderer/Renderer.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>
#include <memory>
#include <stdexcept>

namespace nate::Modules::Render
{
    Model3D::Model3D(Renderer* pRenderer, const std::filesystem::path& file)
        : m_pRenderer(pRenderer)
    {
        if (!std::filesystem::is_regular_file(file))
        {
            throw std::invalid_argument("Invalid file path. " + file.string());
        }

        loadModel(file);
    }

    SquareMatrix4x4<float> Model3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>(0.0, 0.0, 0.0) && m_Origin == Vector3<float>(0.0, 0.0, 0.0))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::translate_init(m_Origin));
        rslt *= SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation);
        return rslt;
    }

    SquareMatrix3x3<float> Model3D::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    void Model3D::Draw(ShaderProgram* pShader)
    {
        pShader->SetShaderVar("model", ModelMatrix());
        pShader->SetShaderVar("norm_mat", NormalMatrix());
        for (auto& obj : m_Objects)
        {
            obj->Draw();
        }
    }

    void Model3D::loadModel(const std::filesystem::path& path)
    {
        Assimp::Importer import;
        const aiScene*   scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || !scene->mRootNode)
        {
            throw std::runtime_error(fmt::format("ERROR::ASSIMP::{}", import.GetErrorString()));
        }
        m_Dir = path.parent_path();

        processNode(scene->mRootNode, scene);
    }

    void Model3D::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Objects.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<Mesh3D> Model3D::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<VertexData>               vertices;
        std::vector<unsigned int>             indices;
        std::vector<std::shared_ptr<Texture>> textures;

        // process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            VertexData vertex;
            vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            if (mesh->HasNormals())
            {
                vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }
            if (mesh->mTextureCoords[0])
            {
                vertex.TextureCoord = {
                    {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y}
                };
            }
            if (mesh->mTangents)
            {
                vertex.TextureTangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
            }
            if (mesh->mBitangents)
            {
                vertex.TextureBitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            }

            vertices.push_back(std::move(vertex));
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        auto pMat = std::make_unique<Material>();

        // process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        if (mesh->mMaterialIndex >= 0)
        {
            pMat->Diffuse  = loadMaterialTextures(material, aiTextureType_DIFFUSE);
            pMat->Specular = loadMaterialTextures(material, aiTextureType_SPECULAR);
            pMat->Normal   = loadMaterialTextures(material, aiTextureType_HEIGHT);
            pMat->Height   = loadMaterialTextures(material, aiTextureType_AMBIENT);
        }

        std::span<float> vertData(
            reinterpret_cast<float*>(vertices.data()),
            reinterpret_cast<float*>(&vertices.back().TextureBitangent[2]));
        auto pObj = std::make_unique<Mesh3D>(
            m_pRenderer,
            VertexData::describe(),
            vertData,
            std::span<std::uint32_t>(indices.begin(), indices.end()));
        pObj->AttachedMaterial(std::move(pMat));

        return pObj;
    }

    std::shared_ptr<Texture> Model3D::loadMaterialTextures(aiMaterial* mat, int type)
    {
        int                                   texUnit{0};
        auto                                  tex_type = static_cast<aiTextureType>(type);
        std::vector<std::shared_ptr<Texture>> textures;
        assert(mat->GetTextureCount(tex_type) <= 1);
        if (mat->GetTextureCount(tex_type) == 0)
            return nullptr;

        for (unsigned int i = 0; i < mat->GetTextureCount(tex_type); i++)
        {
            aiString str;
            mat->GetTexture(tex_type, i, &str);
            std::filesystem::path texPath(str.C_Str());
            texPath           = m_Dir / texPath;
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip         = false;
            auto loadedTexLoc = m_LoadedTextures.find(texPath);
            if (loadedTexLoc != m_LoadedTextures.end())
            {
                textures.push_back(loadedTexLoc->second);
            }
            else
            {
                std::shared_ptr<Texture> pTex = m_pRenderer->CreateTexture(texPath, static_cast<TextureUnit>(texUnit));
                texUnit++;
                m_LoadedTextures.insert({texPath, pTex});
                textures.push_back(pTex);
            }
        }
        // TODO need to support mutliple textures
        return textures[0];
    }
} // namespace nate::Modules::Render
