#pragma once

#include "Camera3D.h"
#include "IWindow.h"
#include "KeyMapping.hpp"
#include "KeyModifiers.hpp"
#include "Keys.h"

#include <StaticMap.hpp>

#include <vector>

namespace nate::Modules::Render
{
    class Fly_Camera3D : public Camera3D
    {
        GUI::KeyMapping m_PanUpMap;
        GUI::KeyMapping m_PanDownMap;
        GUI::KeyMapping m_PanLeftMap;
        GUI::KeyMapping m_PanRightMap;

        GUI::KeyMapping m_RotatePitchMap;
        GUI::KeyMapping m_RotateYawMap;
        GUI::KeyMapping m_RotateRollMap;

        GUI::KeyMapping m_ZoomInMap;
        GUI::KeyMapping m_ZoomOutMap;

        float m_ZoomSpeed{0.1};
        float m_PanSpeed{0.1};
        float m_RotateSpeed{0.1};

        bool IsPanningUp{false};
        bool IsPanningDown{false};
        bool IsPanningLeft{false};
        bool IsPanningRight{false};

      public:
        Fly_Camera3D(GUI::IWindow* pWindow);
        virtual ~Fly_Camera3D() = default;

        void PanUpMap(const GUI::KeyMapping& value) { m_PanUpMap = value; }
        void PanDownMap(const GUI::KeyMapping& value) { m_PanDownMap = value; }
        void PanLeftMap(const GUI::KeyMapping& value) { m_PanLeftMap = value; }
        void PanRightMap(const GUI::KeyMapping& value) { m_PanRightMap = value; }

        const GUI::KeyMapping& PanUpMap() const { return m_PanUpMap; }
        const GUI::KeyMapping& PanDownMap() const { return m_PanDownMap; }
        const GUI::KeyMapping& PanLeftMap() const { return m_PanLeftMap; }
        const GUI::KeyMapping& PanRightMap() const { return m_PanRightMap; }

        void RotatePitchMap(const GUI::KeyMapping& value) { m_RotatePitchMap = value; }
        void RotateYawMap(const GUI::KeyMapping& value) { m_RotateYawMap = value; }
        void RotateRollMap(const GUI::KeyMapping& value) { m_RotateRollMap = value; }

        const GUI::KeyMapping& RotatePitchMap() const { return m_RotatePitchMap; }
        const GUI::KeyMapping& RotateYawMap() const { return m_RotateYawMap; }
        const GUI::KeyMapping& RotateRollMap() const { return m_RotateRollMap; }

        void ZoomInMap(const GUI::KeyMapping& value) { m_ZoomInMap = value; }
        void ZoomOutMap(const GUI::KeyMapping& value) { m_ZoomOutMap = value; }

        const GUI::KeyMapping& ZoomInMap() const { return m_ZoomInMap; }
        const GUI::KeyMapping& ZoomOutMap() const { return m_ZoomOutMap; }

        float ZoomSpeed() const { return m_ZoomSpeed; }
        float PanSpeed() const { return m_PanSpeed; }
        float RotateSpeed() const { return m_RotateSpeed; }

        void ZoomSpeed(float value) { m_ZoomSpeed = value; }
        void PanSpeed(float value) { m_PanSpeed = value; }
        void RotateSpeed(float value) { m_RotateSpeed = value; }

        void PanUp(float value);
        void PanDown(float value);
        void PanLeft(float value);
        void PanRight(float value);

        void RotatePitch(float value);
        void RotateYaw(float value);
        void RotateRoll(float value);

        void ZoomIn(float value);
        void ZoomOut(float value);

        void Update(std::chrono::nanoseconds /* time */) override;

      private:
        void PanUp() { PanUp(m_PanSpeed); }
        void PanDown() { PanDown(m_PanSpeed); }
        void PanLeft() { PanLeft(m_PanSpeed); }
        void PanRight() { PanRight(m_PanSpeed); }

        void RotatePitch() { RotatePitch(m_RotateSpeed); }
        void RotateYaw() { RotateYaw(m_RotateSpeed); }
        void RotateRoll() { RotateRoll(m_RotateSpeed); }

        void ZoomIn() { ZoomIn(m_ZoomSpeed); }
        void ZoomOut() { ZoomOut(m_ZoomSpeed); }

        void ExecuteKeyMappings(const GUI::KeyStateMap& keyStates);
    };
} // namespace nate::Modules::Render