#pragma once

namespace nate::Modules::Render {
    class Vector3D {
      private:
        float m_X;
        float m_Y;
        float m_Z;

      public:
        Vector3D(float x, float y, float z) noexcept
            : m_X(x)
            , m_Y(y)
            , m_Z(z)
        {
            static_assert(sizeof(Vector3D) == 3 * sizeof(float), "Vector3D must be packed tightly");
        }

        Vector3D() noexcept
            : m_X(0.0)
            , m_Y(0.0)
            , m_Z(0.0)
        {
            static_assert(sizeof(Vector3D) == 3 * sizeof(float), "Vector3D must be packed tightly");
        }

        float X() const { return m_X; }
        float Y() const { return m_Y; }
        float Z() const { return m_Z; }

        void X(float value) { m_X = value; }
        void Y(float value) { m_Y = value; }
        void Z(float value) { m_Z = value; }
    };
} // namespace nate::Modules::Render