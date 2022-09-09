#pragma once

#include "Shader/Shader.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    class Renderer;

    class Material
    {
        std::unique_ptr<Shader> m_pFragment;
        std::unique_ptr<Shader> m_pVertex;
        bgfx::ProgramHandle     m_Program;

      public:
        Material(std::unique_ptr<Shader> fragment, std::unique_ptr<Shader> vertex, Renderer* pRenderer);
        ~Material();

        bgfx::ProgramHandle GetProgram() const { return m_Program; }
    };
} // namespace nate::Modules::Render