#pragma once

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
        VertexBuffer_OpenGL(std::span<VertexData> vertexes, std::span<std::uint32_t> indeces);
        VertexBuffer_OpenGL(std::span<VertexData> vertexes);

        ~VertexBuffer_OpenGL() override;

        void Draw() override;

      private:
        void        InitializeVertexData(std::span<VertexData> data);
        void        InitializeIndexData(std::span<std::uint32_t> data);
        void        InitializeVertexArrays();
        static void ClearBindings();
    };
} // namespace nate::Modules::Render
