#pragma once

#include "../Texture/Texture.h"
#include "3D/Mesh3D.h"
#include "Shader/ShaderProgram.h"

#include <filesystem>
#include <map>
#include <memory>
#include <vector>

struct aiNode;
struct aiScene;
class aiMesh;
class aiMaterial;
enum aiTextureType;

namespace nate::Modules::Render
{

    class Renderer;
    class Model3D
    {
        Renderer*                                                 m_pRenderer;
        std::filesystem::path                                     m_Dir;
        std::vector<std::shared_ptr<Mesh3D>>                      m_Objects;
        std::map<std::filesystem::path, std::shared_ptr<Texture>> m_LoadedTextures;

      public:
        Model3D(Renderer* pRenderer, const std::filesystem::path& file);
        virtual ~Model3D() = default;
        virtual void Draw();
        void         Shader(const std::shared_ptr<ShaderProgram>& pShader);

      private:
        void                     loadModel(const std::filesystem::path& path);
        void                     processNode(aiNode* node, const aiScene* scene);
        std::shared_ptr<Mesh3D>  processMesh(aiMesh* mesh, const aiScene* scene);
        std::shared_ptr<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
    };
} // namespace nate::Modules::Render