#include "Material.h"

#include "Renderer.h"
#include "Shader/BGFX_Shader.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    Material::Material(std::unique_ptr<Shader> fragment, std::unique_ptr<Shader> vertex, Renderer* pRenderer)
        : m_pFragment(std::move(fragment))
        , m_pVertex(std::move(vertex))
    {
        m_Program = pRenderer->CreateProgram(
            ((BGFX_Shader*)m_pFragment.get())->Handle(),
            ((BGFX_Shader*)m_pVertex.get())->Handle());
    }

    Material::~Material()
    {
        bgfx::destroy(m_Program);
    }
} // namespace nate::Modules::Render