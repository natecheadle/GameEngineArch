#pragma once

#include "IWindow.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <Units/Radian.hpp>

namespace Ignosi::Libraries::Renderer
{
    class Camera
    {
        Math::Vector3<float> m_Position{0.0, 0.0, 5.0};
        Math::Vector3<float> m_Direction{0.0, 0.0, -1.0};
        Math::Vector3<float> m_WorldUp{0.0, 1.0, 0.0};

        Math::Radian<float> m_FOV{M_PI_4};
        float               m_Near{0.1};
        float               m_Far{100.0};

        IWindow* m_pWindow;

      public:
        Camera(IWindow* pWindow);
        virtual ~Camera();

        virtual Math::SquareMatrix4x4<float> Projection() const;
        virtual Math::SquareMatrix4x4<float> ViewPerspective() const;
        virtual Math::SquareMatrix4x4<float> ViewOrthographic() const;

        const Math::Vector3<float>& CameraPosition() const { return m_Position; }
        const Math::Vector3<float>& CameraDirection() const { return m_Direction; }
        const Math::Vector3<float>& WorldUp() const { return m_WorldUp; }
        const Math::Radian<float>&  FOV() const { return m_FOV; }
        float                       Near() const { return m_Near; }
        float                       Far() const { return m_Far; }

        void CameraPosition(const Math::Vector3<float>& val);
        void CameraDirection(const Math::Vector3<float>& val);
        void WorldUp(const Math::Vector3<float>& val);
        void FOV(const Math::Radian<float>& val) { m_FOV = val; }
        void Near(float val) { m_Near = val; }
        void Far(float val) { m_Far = val; }

        void Translate(const Math::Vector3<float>& value);

      protected:
        const IWindow* Window() const { return m_pWindow; }
    };
} // namespace Ignosi::Libraries::Renderer
