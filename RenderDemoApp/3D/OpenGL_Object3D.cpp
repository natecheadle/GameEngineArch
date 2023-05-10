#include "OpenGL_Object3D.h"

#include <glad/glad.h>

#include <string>
namespace nate::Modules::Render
{
    OpenGL_Object3D::OpenGL_Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces)
        : Object3D(std::move(vertexes), std::move(indeces))
    {
    }

    OpenGL_Object3D::OpenGL_Object3D(std::vector<VertexData> vertexes)
        : Object3D(std::move(vertexes))
    {
    }

    OpenGL_Object3D::~OpenGL_Object3D()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void OpenGL_Object3D::Draw()
    {
        Object3D::Draw();

        glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but
                                  // we'll do so to keep things a bit more organized
        if (Indeces().empty())
        {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(Vertexes().size()));
        }
        else
        {
            glDrawElements(GL_TRIANGLES, static_cast<int>(Vertexes().size()), GL_UNSIGNED_INT, 0);
        }
    }

    void OpenGL_Object3D::InitializeVertexData(const std::vector<VertexData>& data)
    {
        glGenBuffers(1, &m_VBO);

        const std::vector<VertexData>& vertexes = Vertexes();

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(VertexData) * vertexes.size()),
            vertexes.data(),
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

    void OpenGL_Object3D::InitializeIndexData(const std::vector<std::uint32_t>& data)
    {
        glGenBuffers(1, &m_EBO);

        const std::vector<std::uint32_t>& indeces = Indeces();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(std::uint32_t) * indeces.size()),
            indeces.data(),
            GL_STATIC_DRAW);
    }

    void OpenGL_Object3D::InitializeVertexArrays()
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

    void OpenGL_Object3D::ClearBindings()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
} // namespace nate::Modules::Render
