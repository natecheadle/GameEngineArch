#pragma once

#include "Object3D.h"

#include <vector>

namespace nate::Modules::Render
{
    class OpenGL_Object3D : public Object3D
    {
        unsigned int m_VAO;

        unsigned int m_VBO;
        unsigned int m_EBO;

      public:
        OpenGL_Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces);
        OpenGL_Object3D(std::vector<VertexData> vertexes);
        ~OpenGL_Object3D();

        void Draw();

      private:
        void InitializeVertexData(const std::vector<VertexData>& data);
        void InitializeIndexData(const std::vector<std::uint32_t>& data);
        void InitializeVertexArrays();
        void ClearBindings();
    };
} // namespace nate::Modules::Render
