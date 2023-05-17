#pragma once

#include "../Renderer/VertexBuffer.h"
#include "../Shader/ShaderProgram.h"
#include "../Texture/Texture.h"
#include "3D/Material.h"
#include "SquareMatrix4x4.hpp"
#include "VertexData.h"

#include <Vector3.hpp>

#include <initializer_list>
#include <memory>
#include <span>

namespace nate::Modules::Render
{
    class Renderer;

    class Object3D
    {
        std::shared_ptr<ShaderProgram> m_pShader;
        Vector3<float>                 m_Origin;
        Vector3<Radian<float>>         m_Rotation;
        std::shared_ptr<VertexBuffer>  m_pBuffer;
        std::shared_ptr<Material>      m_pMaterial;

        static VertexData m_CubePoints[];

      public:
        Object3D(
            Renderer*                pRenderer,
            const VertexDataConfig&  config,
            std::span<float>         vertexes,
            std::span<std::uint32_t> indeces);
        Object3D(Renderer* pRenderer, const VertexDataConfig& config, std::span<float> vertexes);

        Object3D(const Object3D& other) = default;
        Object3D(Object3D&& other)      = default;

        static std::unique_ptr<Object3D> CreateCube(Renderer* pRenderer);

        virtual ~Object3D() = default;

        void AttachedMaterial(std::shared_ptr<Material> pMat) { m_pMaterial = std::move(pMat); }
        const std::shared_ptr<Material>& AttachedMaterial() const { return m_pMaterial; }

        void Shader(std::shared_ptr<ShaderProgram> pShader) { m_pShader = std::move(pShader); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        virtual void Draw();

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
    };
} // namespace nate::Modules::Render
