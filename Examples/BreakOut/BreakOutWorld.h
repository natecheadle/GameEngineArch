#pragma once

#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <Entity.h>
#include <World.h>

namespace nate::BreakOut
{
    using BreakOutWorld = Modules::ECS::World<Modules::Render::Mesh3D, Modules::Render::Sprite>;
}
