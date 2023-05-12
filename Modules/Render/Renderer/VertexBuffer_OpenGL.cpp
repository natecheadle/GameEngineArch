#include "VertexBuffer_OpenGL.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    VertexBuffer_OpenGL::VertexBuffer_OpenGL(
        const std::vector<VertexData>&    vertexes,
        const std::vector<std::uint32_t>& indeces)
        : m_VertexSize(static_cast<int>(vertexes.size()))
        , m_IndexSize(static_cast<int>(indeces.size()))
    {
        InitializeVertexArrays();
        InitializeVertexData(vertexes);
        InitializeIndexData(indeces);
        ClearBindings();
    }

    VertexBuffer_OpenGL::VertexBuffer_OpenGL(const std::vector<VertexData>& vertexes)
        : m_VertexSize(static_cast<int>(vertexes.size()))
    {
        InitializeVertexArrays();
        InitializeVertexData(vertexes);
        ClearBindings();
    }

    VertexBuffer_OpenGL::~VertexBuffer_OpenGL()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void VertexBuffer_OpenGL::Draw()
    {
        glBindVertexArray(m_VAO);
        if (m_IndexSize == 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, m_VertexSize);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, m_VertexSize, GL_UNSIGNED_INT, 0);
        }
    }

    void VertexBuffer_OpenGL::InitializeVertexData(const std::vector<VertexData>& data)
    {
        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(VertexData) * data.size()),
            data.data(),
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
    }

    void VertexBuffer_OpenGL::InitializeIndexData(const std::vector<std::uint32_t>& data)
    {
        glGenBuffers(1, &m_EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(std::uint32_t) * data.size()),
            data.data(),
            GL_STATIC_DRAW);
    }

    void VertexBuffer_OpenGL::InitializeVertexArrays()
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

    void VertexBuffer_OpenGL::ClearBindings()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
} // namespace nate::Modules::Render