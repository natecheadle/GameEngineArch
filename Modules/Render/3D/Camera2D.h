#pragma once

#include "Camera.h"
#include "IWindow.h"
#include "KeyMapping.hpp"
#include "KeyModifiers.hpp"
#include "Keys.h"

#include <StaticMap.hpp>

#include <vector>

namespace nate::Modules::Render
{
    class Camera2D : public Camera
    {
        GUI::KeyMapping m_PanUpMap;
        GUI::KeyMapping m_PanDownMap;
        GUI::KeyMapping m_PanLeftMap;
        GUI::KeyMapping m_PanRightMap;

        GUI::KeyMapping m_ZoomInMap;
        GUI::KeyMapping m_ZoomOutMap;

        float m_ZoomSpeed{0.1};
        float m_PanSpeed{0.1};

        Radian<float> m_Pitch;
        Radian<float> m_Yaw;
        Radian<float> m_MaxRot{M_PI_2};

      public:
        Camera2D(GUI::IWindow* pWindow);
        virtual ~Camera2D() = default;

        void PanUpMap(const GUI::KeyMapping& value) { m_PanUpMap = value; }
        void PanDownMap(const GUI::KeyMapping& value) { m_PanDownMap = value; }
        void PanLeftMap(const GUI::KeyMapping& value) { m_PanLeftMap = value; }
        void PanRightMap(const GUI::KeyMapping& value) { m_PanRightMap = value; }

        const GUI::KeyMapping& PanUpMap() const { return m_PanUpMap; }
        const GUI::KeyMapping& PanDownMap() const { return m_PanDownMap; }
        const GUI::KeyMapping& PanLeftMap() const { return m_PanLeftMap; }
        const GUI::KeyMapping& PanRightMap() const { return m_PanRightMap; }

        void ZoomInMap(const GUI::KeyMapping& value) { m_ZoomInMap = value; }
        void ZoomOutMap(const GUI::KeyMapping& value) { m_ZoomOutMap = value; }

        const GUI::KeyMapping& ZoomInMap() const { return m_ZoomInMap; }
        const GUI::KeyMapping& ZoomOutMap() const { return m_ZoomOutMap; }

        float ZoomSpeed() const { return m_ZoomSpeed; }
        float PanSpeed() const { return m_PanSpeed; }

        void ZoomSpeed(float value) { m_ZoomSpeed = value; }
        void PanSpeed(float value) { m_PanSpeed = value; }

        void PanUp(float value);
        void PanDown(float value);
        void PanLeft(float value);
        void PanRight(float value);

        void ZoomIn(float value);
        void ZoomOut(float value);

        void Update(double /* dt */) override;

      private:
        void PanUp() { PanUp(m_PanSpeed); }
        void PanDown() { PanDown(m_PanSpeed); }
        void PanLeft() { PanLeft(m_PanSpeed); }
        void PanRight() { PanRight(m_PanSpeed); }

        void ZoomIn() { ZoomIn(m_ZoomSpeed); }
        void ZoomOut() { ZoomOut(m_ZoomSpeed); }

        void ExecuteKeyMappings(const GUI::KeyStateMap& keyStates);
    };
} // namespace nate::Modules::Render