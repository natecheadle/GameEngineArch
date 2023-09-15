#pragma once

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Shader/ShaderProgram.h"
#include "Wall.h"

#include <filesystem>
#include <vector>

namespace Ignosi::Modules::Render
{
    class Renderer;
}

namespace Ignosi::BreakOut
{
    class Level
    {
        std::vector<Brick>         m_Bricks;
        std::vector<Wall>          m_Walls;
        BreakOutWorld*             m_pWorld;
        Modules::Render::Renderer* m_pRenderer;

      public:
        Level(BreakOutWorld* pWorld, Modules::Render::Renderer* pRenderer);

        void Load(const std::filesystem::path& file, unsigned int lvlWidth, unsigned int lvlHeight);
        bool IsCompleted();

        void Draw(Modules::Render::ShaderProgram* pProgram);

      private:
        void Initialize(std::vector<std::vector<unsigned int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight);
    };
} // namespace Ignosi::BreakOut
