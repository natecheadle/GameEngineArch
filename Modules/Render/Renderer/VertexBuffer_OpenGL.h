#pragma once

#include "Renderer/VertexDataConfig.h"
#include "VertexBuffer.h"

#include <span>

namespace nate::Modules::Render
{
    class VertexBuffer_OpenGL : public VertexBuffer
    {

        unsigned int m_VAO{0};
        unsigned int m_VBO{0};
        unsigned int m_EBO{0};

        int m_VertexSize{0};
        int m_IndexSize{0};

      public:
        VertexBuffer_OpenGL(
            const VertexDataConfig&        config,
            std::span<const float>         vertexData,
            std::span<const std::uint32_t> indeces);
        VertexBuffer_OpenGL(const VertexDataConfig& config, std::span<const float> vertexData);

        ~VertexBuffer_OpenGL() override;

        void Draw() override;

      private:
        void        InitializeVertexData(const VertexDataConfig& config, std::span<const float> vertexData);
        void        InitializeIndexData(std::span<const std::uint32_t> data);
        void        InitializeVertexArrays();
        static void ClearBindings();

        static int VertexSizeInit(const VertexDataConfig& config, std::span<const float> vertexData);
        static int ConvertType(VertexDataConfig::DataType type);
    };
} // namespace nate::Modules::Render
