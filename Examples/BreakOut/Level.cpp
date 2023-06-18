#include "Level.h"

#include "3D/Material.h"
#include "3D/RGB_Color.h"
#include "3D/Sprite.h"
#include "Physics/RigidBody2D.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <utility>

using namespace nate::Modules;

namespace nate::BreakOut
{

    Level::Level(BreakOutWorld* pWorld, Modules::Render::Renderer* pRenderer)
        : m_pWorld(pWorld)
        , m_pRenderer(pRenderer)
    {
    }

    void Level::Load(const std::filesystem::path& file, unsigned int lvlWidth, unsigned int lvlHeight)
    {
        assert(std::filesystem::is_regular_file(file));
        // clear old data
        m_Bricks.clear();
        // load from file
        unsigned int                           tileCode{0};
        std::string                            line;
        std::ifstream                          fstream(file);
        std::vector<std::vector<unsigned int>> tileData;
        if (fstream)
        {
            while (std::getline(fstream, line)) // read each line from level file
            {
                std::istringstream        sstream(line);
                std::vector<unsigned int> row;
                while (sstream >> tileCode) // read each word separated by spaces
                    row.push_back(tileCode);
                tileData.push_back(row);
            }
            if (!tileData.empty())
                Initialize(tileData, lvlWidth, lvlHeight);
        }

        float wall_thickness = 50.0;
        float lvl_width_flt  = static_cast<float>(lvlWidth);
        float lvl_height_flt = static_cast<float>(lvlHeight);

        Wall top(m_pWorld->CreateEntity<Wall>(Physics::RigidBody2D()));
        Wall left(m_pWorld->CreateEntity<Wall>(Physics::RigidBody2D()));
        Wall right(m_pWorld->CreateEntity<Wall>(Physics::RigidBody2D()));
        left.Position({-0.5f * wall_thickness, lvl_width_flt / 2.0f});
        right.Position({0.5f * wall_thickness + lvl_width_flt, lvl_width_flt / 2.0f});
        top.Position({lvl_width_flt / 2.0f, -0.5f * wall_thickness});

        top.HitBox({lvl_width_flt, wall_thickness});
        left.HitBox({wall_thickness, lvl_height_flt});
        right.HitBox({wall_thickness, lvl_height_flt});

        m_Walls.push_back(std::move(top));
        m_Walls.push_back(std::move(left));
        m_Walls.push_back(std::move(right));
    }

    void Level::Draw(Modules::Render::ShaderProgram* pProgram)
    {
        for (auto& brick : m_Bricks)
        {
            brick.Draw(pProgram);
        }
    }

    void Level::Initialize(
        std::vector<std::vector<unsigned int>> tileData,
        unsigned int                           lvlWidth,
        unsigned int                           lvlHeight)
    { // calculate dimensions
        unsigned int height      = tileData.size();
        unsigned int width       = tileData[0].size();
        float        unit_width  = static_cast<float>(lvlWidth) / static_cast<float>(width);
        float        unit_height = static_cast<float>(lvlHeight) / static_cast<float>(height);

        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        shader_dir /= "Shaders";

        auto solid_block_path = shader_dir / "block_solid.png";
        auto block_path       = shader_dir / "block.png";

        std::shared_ptr<Render::Material> pBlockMat      = std::make_shared<Render::Material>();
        std::shared_ptr<Render::Material> pSolidBlockMat = std::make_shared<Render::Material>();

        pBlockMat->Diffuse      = m_pRenderer->CreateTexture(block_path, Render::TextureUnit::Texture0);
        pSolidBlockMat->Diffuse = m_pRenderer->CreateTexture(solid_block_path, Render::TextureUnit::Texture0);

        // initialize level tiles based on tileData
        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                Render::Sprite sprite(m_pRenderer);

                // check block type from level data (2D level array)
                if (tileData[y][x] == 1) // solid
                {
                    Vector2<float> pos({unit_width * static_cast<float>(x), unit_height * static_cast<float>(y)});
                    Vector2<float> size({unit_width, unit_height});
                    sprite.AttachedMaterial(pSolidBlockMat);
                    sprite.Size(size);
                    sprite.Color({0.8f, 0.8f, 0.7f});
                    Brick brick(m_pWorld->CreateEntity<Brick>(std::move(sprite), Physics::RigidBody2D()));
                    brick.Position(pos);
                    brick.HitBox({static_cast<float>(width), static_cast<float>(height)});
                    brick.Type(BrickType::Solid);
                    m_Bricks.push_back(std::move(brick));
                }
                else if (tileData[y][x] > 1)
                {
                    Render::RGB_Color color = Render::RGB_Color(1.0f, 1.0f, 1.0f); // original: white
                    if (tileData[y][x] == 2)
                        color = Render::RGB_Color(0.2f, 0.6f, 1.0f);
                    else if (tileData[y][x] == 3)
                        color = Render::RGB_Color(0.0f, 0.7f, 0.0f);
                    else if (tileData[y][x] == 4)
                        color = Render::RGB_Color(0.8f, 0.8f, 0.4f);
                    else if (tileData[y][x] == 5)
                        color = Render::RGB_Color(1.0f, 0.5f, 0.0f);

                    Vector2<float> pos({unit_width * static_cast<float>(x), unit_height * static_cast<float>(y)});
                    Vector2<float> size({unit_width, unit_height});

                    sprite.AttachedMaterial(pBlockMat);
                    sprite.Size(size);
                    sprite.Color(color);

                    Brick brick(m_pWorld->CreateEntity<Brick>(std::move(sprite), Physics::RigidBody2D()));
                    brick.Position(pos);
                    brick.HitBox({static_cast<float>(width), static_cast<float>(height)});
                    m_Bricks.push_back(std::move(brick));
                }
            }
        }
    }
} // namespace nate::BreakOut
