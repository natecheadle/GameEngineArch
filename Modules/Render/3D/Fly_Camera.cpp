#include "Fly_Camera.h"
#include "IWindow.h"
#include "KeyModifiers.hpp"
#include "KeyPressedInfo.hpp"
#include "Keys.h"
#include "Units/Angle.hpp"
#include "Vector3.hpp"

#include <functional>


namespace nate::Modules::Render
{
    Fly_Camera::Fly_Camera(GUI::IWindow* pWindow)
        : Camera(pWindow)
        , m_PanUpMap([this]() { PanUp(); })
        , m_PanDownMap([this]() { PanDown(); })
        , m_PanLeftMap([this]() { PanLeft(); })
        , m_PanRightMap([this]() { PanRight(); })
        , m_RotatePitchPosMap([this]() { RotatePitchPos(); })
        , m_RotatePitchNegMap([this]() { RotatePitchNeg(); })
        , m_RotateYawPosMap([this]() { RotateYawPos(); })
        , m_RotateYawNegMap([this]() { RotateYawNeg(); })
        , m_ZoomInMap([this]() { ZoomIn(); })
        , m_ZoomOutMap([this]() { ZoomOut(); })
        , m_Pitch(0.0)
        , m_Yaw(-M_PI_2)
    {
        m_PanUpMap.KeyMappings({
            {{GUI::Key::W, GUI::KeyModifiers()}, {GUI::Key::Up, GUI::KeyModifiers()}}
        });
        m_PanDownMap.KeyMappings({
            {{GUI::Key::S, GUI::KeyModifiers()}, {GUI::Key::Down, GUI::KeyModifiers()}}
        });
        m_PanLeftMap.KeyMappings({
            {{GUI::Key::A, GUI::KeyModifiers()}, {GUI::Key::Left, GUI::KeyModifiers()}}
        });
        m_PanRightMap.KeyMappings({
            {{GUI::Key::D, GUI::KeyModifiers()}, {GUI::Key::Right, GUI::KeyModifiers()}}
        });

        m_RotatePitchPosMap.KeyMappings({
            {{GUI::Key::C, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_RotatePitchNegMap.KeyMappings({
            {{GUI::Key::R, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });

        m_RotateYawPosMap.KeyMappings({
            {{GUI::Key::Q, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_RotateYawNegMap.KeyMappings({
            {{GUI::Key::E, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });

        m_ZoomInMap.KeyMappings({
            {{GUI::Key::Z, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_ZoomOutMap.KeyMappings({
            {{GUI::Key::X, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
    }

    void Fly_Camera::PanUp(float value)
    {
        Translate({0, -value, 0});
    }

    void Fly_Camera::PanDown(float value)
    {
        Translate({0, value, 0});
    }

    void Fly_Camera::PanLeft(float value)
    {
        Translate({value, 0, 0});
    }

    void Fly_Camera::PanRight(float value)
    {
        Translate({-value, 0, 0});
    }

    void Fly_Camera::RotatePitch(const Radian<float>& value)
    {
        m_Pitch += value;
        m_Pitch = m_Pitch >= m_MaxRot ? m_MaxRot : m_Pitch;
        CameraDirection(CalcDir());
    }

    void Fly_Camera::RotateYaw(const Radian<float>& value)
    {
        m_Yaw += value;
        m_Yaw = m_Yaw >= m_MaxRot ? m_MaxRot : m_Yaw;
        CameraDirection(CalcDir());
    }

    void Fly_Camera::ZoomIn(float value)
    {
        Translate({0, 0, value});
    }

    void Fly_Camera::ZoomOut(float value)
    {
        Translate({0, 0, -value});
    }

    void Fly_Camera::Update(std::chrono::nanoseconds /* time */)
    {
        Window()->ExecuteWithKeyStates([this](const GUI::KeyStateMap& keyStates) { ExecuteKeyMappings(keyStates); });
    }

    Vector3<float> Fly_Camera::CalcDir() const
    {
        Vector3<float> dir;
        dir.x(cos(m_Yaw) * cos(m_Pitch));
        dir.y(sin(m_Pitch));
        dir.z(sin(m_Yaw) * cos(m_Pitch));

        return dir;
    }

    void Fly_Camera::ExecuteKeyMappings(const GUI::KeyStateMap& keyStates)
    {
        auto mappingUpdate = [keyStates](const GUI::KeyMapping& mappings) -> void {
            for (const auto& mapping : mappings.KeyMappings())
            {
                if ((keyStates[mapping.first].first == GUI::KeyState::Pressed ||
                     keyStates[mapping.first].first == GUI::KeyState::Repeat) &&
                    keyStates[mapping.first].second == mapping.second)
                {
                    mappings.ExecuteKeyEvent();
                    return;
                }
            }
        };

        mappingUpdate(m_PanUpMap);
        mappingUpdate(m_PanDownMap);
        mappingUpdate(m_PanLeftMap);
        mappingUpdate(m_PanRightMap);

        mappingUpdate(m_RotatePitchPosMap);
        mappingUpdate(m_RotatePitchNegMap);
        mappingUpdate(m_RotateYawPosMap);
        mappingUpdate(m_RotateYawNegMap);

        mappingUpdate(m_ZoomInMap);
        mappingUpdate(m_ZoomOutMap);
    }
} // namespace nate::Modules::Render
