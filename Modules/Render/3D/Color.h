#pragma once

#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Modules::Render
{
    class Color
    {
        Vector3<float> m_Value;

      protected:
        Color() noexcept = default;
        Color(Vector3<float> val) noexcept
            : m_Value(std::move(val))
        {
        }

      public:
        const Vector3<float>& Data() const { return m_Value; }
        Vector3<float>&       Data() { return m_Value; }
        void                  Data(const Vector3<float>& val) { m_Value = val; }
    };
} // namespace Ignosi::Modules::Render
