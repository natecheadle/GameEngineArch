#pragma once

#include "VertexBuffer.h"

#include <vector>

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
        VertexBuffer_OpenGL(const std::vector<VertexData>& vertexes, const std::vector<std::uint32_t>& indeces);
        VertexBuffer_OpenGL(const std::vector<VertexData>& vertexes);

        ~VertexBuffer_OpenGL() override;

        void Draw() override;

      private:
        void        InitializeVertexData(const std::vector<VertexData>& data);
        void        InitializeIndexData(const std::vector<std::uint32_t>& data);
        void        InitializeVertexArrays();
        static void ClearBindings();
    };
} // namespace nate::Modules::Render
