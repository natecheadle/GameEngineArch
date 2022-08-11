#include "Fly_Camera3D.h"

#include "3D/Camera/Camera3D.h"
#include "KeyModifiers.hpp"
#include "KeyPressedInfo.hpp"
#include "Keys.h"

namespace nate::Modules::Render {
    Fly_Camera3D::Fly_Camera3D(GUI::IWindow* pWindow)
        : Camera3D(pWindow)
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

        m_RotatePitchMap.KeyMappings({
            {{GUI::Key::Q, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_RotateYawMap.KeyMappings({
            {{GUI::Key::E, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });

        m_ZoomInMap.KeyMappings({
            {{GUI::Key::Z, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_ZoomOutMap.KeyMappings({
            {{GUI::Key::X, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
    }

    void Fly_Camera3D::PanUp(float value)
    {
        Translate({value, 0, 0});
    }

    void Fly_Camera3D::PanDown(float value)
    {
        Translate({-value, 0, 0});
    }

    void Fly_Camera3D::PanLeft(float value)
    {
        Translate({0, -value, 0});
    }

    void Fly_Camera3D::PanRight(float value)
    {
        Translate({0, value, 0});
    }

    void Fly_Camera3D::RotatePitch(float value)
    {
        Rotate({value, 0, 0});
    }

    void Fly_Camera3D::RotateYaw(float value)
    {
        Rotate({0, value, 0});
    }

    void Fly_Camera3D::RotateRoll(float value)
    {
        Rotate({0, 0, value});
    }

    void Fly_Camera3D::ZoomIn(float value)
    {
        Translate({0, 0, value});
    }

    void Fly_Camera3D::ZoomOut(float value)
    {
        Translate({0, 0, -value});
    }

    void Fly_Camera3D::OnKeyPressed(const GUI::KeyPressedInfo& value)
    {
        GUI::Key          key  = value.Key();
        GUI::KeyModifiers mods = value.KeyMods();

        if (m_PanDownMap.IsMappedKey(key, mods))
        {
            PanDown();
        }
        if (m_PanUpMap.IsMappedKey(key, mods))
        {
            PanUp();
        }
        if (m_PanLeftMap.IsMappedKey(key, mods))
        {
            PanLeft();
        }
        if (m_PanRightMap.IsMappedKey(key, mods))
        {
            PanRight();
        }
    }
} // namespace nate::Modules::Render
