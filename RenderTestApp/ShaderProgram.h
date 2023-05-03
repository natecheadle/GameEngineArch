#pragma once

#include "Shader.h"

#include <memory>

namespace nate::Modules::Render
{
    class ShaderProgram
    {
        unsigned int            m_ID;
        std::shared_ptr<Shader> m_pFragmentShader;
        std::shared_ptr<Shader> m_pGeometryShader;
        std::shared_ptr<Shader> m_pVertexShader;

        bool        m_IsValid;
        std::string m_Error;

      public:
        ShaderProgram(
            std::shared_ptr<Shader> pFragmentShader,
            std::shared_ptr<Shader> pGeometryShader,
            std::shared_ptr<Shader> pVertexShader);

        ~ShaderProgram();

        void         Use();
        unsigned int ID() const { return m_ID; }

      protected:
        void SetShaderVar(const std::string& name, bool value) const;
        void SetShaderVar(const std::string& name, int value) const;
        void SetShaderVar(const std::string& name, float value) const;
    };
} // namespace nate::Modules::Render