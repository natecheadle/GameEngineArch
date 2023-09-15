#include "Level.h"

#include "3D/Material.h"
#include "3D/RGB_Color.h"
#include "3D/Sprite.h"
#include "Brick.h"
#include "HitRectangle.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <utility>

using namespace Ignosi::Modules;

namespace Ignosi::BreakOut
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

        Wall top(m_pWorld->CreateEntity<Wall>());
        Wall left(m_pWorld->CreateEntity<Wall>());
        Wall right(m_pWorld->CreateEntity<Wall>());
        left.KinematicData()->Position({-0.5f * wall_thickness, lvl_width_flt / 2.0f});
        right.KinematicData()->Position({0.5f * wall_thickness + lvl_width_flt, lvl_width_flt / 2.0f});
        top.KinematicData()->Position({lvl_width_flt / 2.0f, -0.5f * wall_thickness});

        top.Body()->HitShape(std::make_unique<Modules::Physics::HitRectangle>(top.KinematicData(), lvl_width_flt, wall_thickness));
        left.Body()->HitShape(std::make_unique<Modules::Physics::HitRectangle>(left.KinematicData(), wall_thickness, lvl_height_flt));
        right.Body()->HitShape(std::make_unique<Modules::Physics::HitRectangle>(right.KinematicData(), wall_thickness, lvl_height_flt));

        m_Walls.push_back(std::move(top));
        m_Walls.push_back(std::move(left));
        m_Walls.push_back(std::move(right));
    }

    void Level::Draw(Modules::Render::ShaderProgram* pProgram) {}

    void Level::Initialize(std::vector<std::vector<unsigned int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight)
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
                Vector2<float> pos({unit_width * static_cast<float>(x), unit_height * static_cast<float>(y)});
                Vector2<float> size({unit_width, unit_height});

                Brick brick(m_pWorld->CreateEntity(), m_pRenderer);
                brick.Sprite()->AttachedMaterial(pSolidBlockMat);
                brick.Sprite()->Size(size);
                brick.KinematicData()->Position(pos);
                auto pHitRectangle = std::make_unique<Physics::HitRectangle>(brick.KinematicData());
                pHitRectangle->Height(unit_height);
                pHitRectangle->Width(unit_width);
                brick.Body()->HitShape(std::move(pHitRectangle));
                brick.Type(static_cast<BrickType>(tileData[x][y]));

                // check block type from level data (2D level array)
                if (tileData[y][x] == 1) // solid
                {
                    brick.Sprite()->Color({0.8f, 0.8f, 0.7f});
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

                    brick.Sprite()->AttachedMaterial(pBlockMat);
                }
                m_Bricks.push_back(std::move(brick));
            }
        }
    }
} // namespace Ignosi::BreakOut
