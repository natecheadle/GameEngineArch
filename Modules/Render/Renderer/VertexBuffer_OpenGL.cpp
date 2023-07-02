#include "VertexBuffer_OpenGL.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    VertexBuffer_OpenGL::VertexBuffer_OpenGL(
        const VertexDataConfig&        config,
        std::span<const float>         vertexData,
        std::span<const std::uint32_t> indeces)
        : m_VertexSize(static_cast<int>(vertexData.size()))
        , m_IndexSize(static_cast<int>(indeces.size()))
    {
        InitializeVertexArrays();
        InitializeVertexData(config, vertexData);
        InitializeIndexData(indeces);
        ClearBindings();
    }

    VertexBuffer_OpenGL::VertexBuffer_OpenGL(const VertexDataConfig& config, std::span<const float> vertexData)
        : m_VertexSize(static_cast<int>(vertexData.size()))
    {
        InitializeVertexArrays();
        InitializeVertexData(config, vertexData);
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
        glBindVertexArray(0);
    }

    void VertexBuffer_OpenGL::InitializeVertexData(const VertexDataConfig& configs, std::span<const float> vertexData)
    {
        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<std::uint32_t>(sizeof(float) * vertexData.size()),
            vertexData.data(),
            GL_STATIC_DRAW);

        int    i{0};
        size_t cur_offset{0};
        for (const auto& config : configs.Describe())
        {
            glVertexAttribPointer(
                i,
                config.Length,
                ConvertType(config.BaseType),
                GL_FALSE,
                static_cast<GLsizei>(configs.ConfigSize()),
                (void*)cur_offset);
            glEnableVertexAttribArray(i);
            cur_offset += config.TotalSize();
            i++;
        }
    }

    void VertexBuffer_OpenGL::InitializeIndexData(std::span<const std::uint32_t> data)
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

    int VertexBuffer_OpenGL::ConvertType(VertexDataConfig::DataType type)
    {
        switch (type)
        {
        case VertexDataConfig::DataType::INT32: return GL_INT;
        case VertexDataConfig::DataType::UINT32: return GL_UNSIGNED_INT;
        case VertexDataConfig::DataType::FLOAT32: return GL_FLOAT;
        default: assert(false); return 0;
        }
    }
} // namespace Ignosi::Modules::Render