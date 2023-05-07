#pragma once

#include "Shader.h"
#include "SquareMatrix4x4.hpp"

#include <memory>

namespace nate::Modules::Render
{
    class ShaderProgram
    {
        unsigned int m_ID;

      public:
        ShaderProgram(const Shader* pFragmentShader, const Shader* pGeometryShader, const Shader* pVertexShader);

        ~ShaderProgram();

        void         Use() const;
        unsigned int ID() const { return m_ID; }

        void SetShaderVar(const std::string& name, bool value) const;
        void SetShaderVar(const std::string& name, int value) const;
        void SetShaderVar(const std::string& name, float value) const;
        void SetShaderVar(const std::string& name, const SquareMatrix<4, float>& value) const;
    };
} // namespace nate::Modules::Render