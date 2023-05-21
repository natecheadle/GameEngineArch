#pragma once

#include "Ball.h"
#include "BreakOutWorld.h"
#include "Brick.h"
#include "Paddle.h"

#include <filesystem>
#include <vector>

namespace nate::Modules::Render
{
    class Renderer;
}

namespace nate::BreakOut
{
    class Level
    {
        std::vector<Brick>         m_Bricks;
        std::vector<Ball>          m_Balls;
        Paddle                     m_Paddle;
        BreakOutWorld*             m_pWorld;
        Modules::Render::Renderer* m_pRenderer;

      public:
        Level(BreakOutWorld* pWorld, Modules::Render::Renderer* pRenderer);

        void Load(const std::filesystem::path& file, unsigned int lvlWidth, unsigned int lvlHeight);
        bool IsCompleted();

      private:
        void Initialize(std::vector<std::vector<unsigned int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight);
    };
} // namespace nate::BreakOut
