#pragma once

#include "../Texture/Texture.h"
#include "3D/Mesh3D.h"
#include "KinematicData.h"
#include "Shader/ShaderProgram.h"
#include "WeakComponentPointer.h"

#include <filesystem>
#include <map>
#include <memory>
#include <vector>

struct aiNode;
struct aiScene;
class aiMesh;
class aiMaterial;

namespace Ignosi::Modules::Render
{

    class Renderer;
    class Model3D
    {
        Renderer*                                                 m_pRenderer;
        ECS::WeakComponentPointer<Physics::KinematicData>         m_pPosition;
        std::filesystem::path                                     m_Dir;
        std::vector<std::shared_ptr<Mesh3D>>                      m_Objects;
        std::map<std::filesystem::path, std::shared_ptr<Texture>> m_LoadedTextures;

        Vector3<float>                m_Origin;
        Vector3<Radian<float>>        m_Rotation;
        std::shared_ptr<VertexBuffer> m_pBuffer;

      public:
        Model3D(Renderer* pRenderer, ECS::WeakComponentPointer<Physics::KinematicData> pPosition, const std::filesystem::path& file);
        virtual ~Model3D() = default;
        virtual void Draw(ShaderProgram* pShader);

        SquareMatrix4x4<float> ModelMatrix() const;
        SquareMatrix3x3<float> NormalMatrix() const;

        void                  Origin(const Vector3<float>& val) { m_Origin = val; }
        const Vector3<float>& Origin() const { return m_Origin; }

        void Translate(const Vector3<float>& val) { m_Origin += val; }
        void Rotate(const Vector3<Radian<float>>& val) { m_Rotation += val; }

        void TranslateX(float val) { m_Origin[0] += val; }
        void TranslateY(float val) { m_Origin[1] += val; }
        void TranslateZ(float val) { m_Origin[2] += val; }

        void RotX(const Radian<float>& val) { m_Rotation[0] += val; }
        void RotY(const Radian<float>& val) { m_Rotation[1] += val; }
        void RotZ(const Radian<float>& val) { m_Rotation[2] += val; }

      private:
        void                     loadModel(const std::filesystem::path& path);
        void                     processNode(aiNode* node, const aiScene* scene);
        std::shared_ptr<Mesh3D>  processMesh(aiMesh* mesh, const aiScene* scene);
        std::shared_ptr<Texture> loadMaterialTextures(aiMaterial* mat, int type);
    };
} // namespace Ignosi::Modules::Render