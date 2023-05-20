#pragma once

#include "3D/Sprite.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "World.h"

#include <filesystem>
#include <vector>

namespace nate::BreakOut
{
    class Level
    {
        std::vector<Brick> m_Bricks;
        std::vector<Ball>  m_Ballls;
        Paddle             m_Paddle;

      public:
        Level(Modules::ECS::World<Modules::Render::Sprite>* pWorld);

        void Load(const std::filesystem::path& file);
        bool IsCompleted();

      private:
        void Initialize(std::vector<std::vector<unsigned int>> tileData);
    };
} // namespace nate::BreakOut
