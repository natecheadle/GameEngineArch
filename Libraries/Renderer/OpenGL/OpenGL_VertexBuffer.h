#pragma once

#include "IVertexBuffer.h"
#include "VertexDataConfig.h"

#include <span>

namespace Ignosi::Libraries::Renderer
{
    class OpenGL_VertexBuffer : public IVertexBuffer
    {
        unsigned int m_VAO{0};
        unsigned int m_VBO{0};
        unsigned int m_EBO{0};

        int m_VertexSize{0};
        int m_IndexSize{0};

      public:
        OpenGL_VertexBuffer(const VertexDataConfig& config, std::span<const float> vertexData, std::span<const std::uint32_t> indeces);
        OpenGL_VertexBuffer(const VertexDataConfig& config, std::span<const float> vertexData);

        ~OpenGL_VertexBuffer() override;

        void Draw() const override;

      private:
        void        InitializeVertexData(const VertexDataConfig& config, std::span<const float> vertexData);
        void        InitializeIndexData(std::span<const std::uint32_t> data);
        void        InitializeVertexArrays();
        static void ClearBindings();

        static int ConvertType(VertexDataConfig::DataType type);
    };
} // namespace Ignosi::Libraries::Renderer
