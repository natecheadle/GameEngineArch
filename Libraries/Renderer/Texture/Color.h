#pragma once

#include <LinearAlgebra/Vector.hpp>

namespace Ignosi::Libraries::Renderer
{
    class Color
    {
        Math::Vector3<float> m_Value;

      protected:
        Color() noexcept = default;
        Color(Math::Vector3<float> val) noexcept
            : m_Value(std::move(val))
        {
        }

      public:
        const Math::Vector3<float>& Data() const { return m_Value; }
        Math::Vector3<float>&       Data() { return m_Value; }
        void                        Data(const Math::Vector3<float>& val) { m_Value = val; }
    };
} // namespace Ignosi::Libraries::Renderer
