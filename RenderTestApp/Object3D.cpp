#include "Object3D.h"

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

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
        // vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the
        // VAO; keep the EBO bound. glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
        // happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs
        // (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    Object3D::~Object3D()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Object3D::Draw()
    {

        m_pShader->Use();

        glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but
                                  // we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time}
    }
} // namespace nate::Modules::Render
