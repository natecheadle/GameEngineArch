#pragma once

#include "3D/Material.h"
#include "KinematicData.h"
#include "Renderer/VertexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"
#include "Units/Radian.hpp"
#include "VertexData.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <span>

namespace Ignosi::Modules::Render
{
    class Renderer;

    class Mesh3D
    {

        Renderer*                      m_pRenderer{nullptr};
        Physics::KinematicData*        m_pPosition;
        std::shared_ptr<VertexBuffer>  m_pBuffer{nullptr};
        std::shared_ptr<ShaderProgram> m_pShader{nullptr};
        std::shared_ptr<Material>      m_pMaterial{nullptr};

        static const VertexData m_CubePoints[];

      public:
        Mesh3D() = default;
        Mesh3D(
            Renderer*                      pRenderer,
            Physics::KinematicData*        pPosition,
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces);
        Mesh3D(Renderer* pRenderer, Physics::KinematicData* pPosition, const VertexDataConfig& config, std::span<const float> vertexes);

        Mesh3D(const Mesh3D& other)     = default;
        Mesh3D(Mesh3D&& other) noexcept = default;

        Mesh3D& operator=(const Mesh3D& other)     = default;
        Mesh3D& operator=(Mesh3D&& other) noexcept = default;

        static Mesh3D CreateCube(Renderer* pRenderer, Physics::KinematicData* pPosition);

        virtual ~Mesh3D() = default;

        void                                  Shader(std::shared_ptr<ShaderProgram> pProgram) { m_pShader = std::move(pProgram); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        void                             AttachedMaterial(std::shared_ptr<Material> pMat) { m_pMaterial = std::move(pMat); }
        const std::shared_ptr<Material>& AttachedMaterial() const { return m_pMaterial; }

        virtual void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;
        SquareMatrix3x3<float> NormalMatrix() const;

        Vector3<float> Origin() const { return m_pPosition->Position(); }
        Radian<float>  Rotation() const { return m_pPosition->Angle(); }
    };
} // namespace Ignosi::Modules::Render
