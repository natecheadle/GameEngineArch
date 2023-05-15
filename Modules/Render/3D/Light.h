#include <Vector3.hpp>

namespace nate::Modules::Render
{
    struct Light
    {
        Vector3<float> Position;

        Vector3<float> Ambient;
        Vector3<float> Diffuse;
        Vector3<float> Specular;
    };
} // namespace nate::Modules::Render
