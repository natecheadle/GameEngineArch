#pragma once

#include "Camera3D.h"
#include "IWindow.h"
#include "KeyMapping.hpp"
#include "KeyModifiers.hpp"
#include "Keys.h"
#include "Units/Radian.hpp"

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

        GUI::KeyMapping m_RotatePitchPosMap;
        GUI::KeyMapping m_RotatePitchNegMap;
        GUI::KeyMapping m_RotateYawPosMap;
        GUI::KeyMapping m_RotateYawNegMap;

        GUI::KeyMapping m_ZoomInMap;
        GUI::KeyMapping m_ZoomOutMap;

        float m_ZoomSpeed{0.1};
        float m_PanSpeed{0.1};
        float m_RotateSpeed{M_PI_2 / 100.0};

        bool IsPanningUp{false};
        bool IsPanningDown{false};
        bool IsPanningLeft{false};
        bool IsPanningRight{false};

        Radian<float> m_Pitch;
        Radian<float> m_Yaw;
        Radian<float> m_MaxRot{M_PI_2};

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

        void RotatePitchPosMap(const GUI::KeyMapping& value) { m_RotatePitchPosMap = value; }
        void RotatePitchNegMap(const GUI::KeyMapping& value) { m_RotatePitchNegMap = value; }
        void RotateYawPosMap(const GUI::KeyMapping& value) { m_RotateYawPosMap = value; }
        void RotateYawPosNegMap(const GUI::KeyMapping& value) { m_RotateYawNegMap = value; }

        const GUI::KeyMapping& RotatePitchPosMap() const { return m_RotatePitchPosMap; }
        const GUI::KeyMapping& RotatePitchNegMap() const { return m_RotatePitchNegMap; }
        const GUI::KeyMapping& RotateYawPosMap() const { return m_RotateYawPosMap; }
        const GUI::KeyMapping& RotateYawNegMap() const { return m_RotateYawNegMap; }

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

        void RotatePitch(const Radian<float>& value);
        void RotateYaw(const Radian<float>& value);

        void ZoomIn(float value);
        void ZoomOut(float value);

        void Update(std::chrono::nanoseconds /* time */) override;

      private:
        Vector3<float> CalcDir() const;

        void PanUp() { PanUp(m_PanSpeed); }
        void PanDown() { PanDown(m_PanSpeed); }
        void PanLeft() { PanLeft(m_PanSpeed); }
        void PanRight() { PanRight(m_PanSpeed); }

        void RotatePitchPos() { RotatePitch(m_RotateSpeed); }
        void RotatePitchNeg() { RotatePitch(-m_RotateSpeed); }
        void RotateYawPos() { RotateYaw(m_RotateSpeed); }
        void RotateYawNeg() { RotateYaw(-m_RotateSpeed); }

        void ZoomIn() { ZoomIn(m_ZoomSpeed); }
        void ZoomOut() { ZoomOut(m_ZoomSpeed); }

        void ExecuteKeyMappings(const GUI::KeyStateMap& keyStates);
    };
} // namespace nate::Modules::Render