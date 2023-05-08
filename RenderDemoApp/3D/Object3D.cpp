#include "Object3D.h"

#include "SquareMatrix4x4.hpp"
#include "Vector3.hpp"

#include <OpenGL/OpenGL.h>
#include <glad/glad.h>

#include <algorithm>
#include <cstddef>

namespace nate::Modules::Render
{
    Object3D::Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces)
        : m_Vertexes(std::move(vertexes))
        , m_Indeces(std::move(indeces))
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
        // attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(VertexData) * m_Vertexes.size()),
            m_Vertexes.data(),
            GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(std::uint32_t) * m_Indeces.size()),
            m_Indeces.data(),
            GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Position));
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Color));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, TextureCoord));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Object3D::Object3D(std::vector<VertexData> vertexes)
        : m_Vertexes(std::move(vertexes))
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(VertexData) * m_Vertexes.size()),
            m_Vertexes.data(),
            GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Position));
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Color));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, TextureCoord));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Object3D::~Object3D()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    SquareMatrix4x4<float> Object3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>({0.0, 0.0, 0.0}) && m_Origin == Vector3<float>({0.0, 0.0, 0.0}))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation));
        rslt *= SquareMatrix4x4<float>::translate_init(m_Origin);
        return rslt;
    }

    void Object3D::Draw()
    {
        for (auto& tex : m_Textures)
        {
            if (tex)
            {
                tex->Activate();
                tex->Bind();
            }
        }
        m_pShader->Use();

        glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but
                                  // we'll do so to keep things a bit more organized
        if (m_Indeces.empty())
        {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_Vertexes.size()));
        }
        else
        {
            glDrawElements(GL_TRIANGLES, static_cast<int>(m_Vertexes.size()), GL_UNSIGNED_INT, 0);
        }
    }
} // namespace nate::Modules::Render
