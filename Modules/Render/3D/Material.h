#include <Vector3.hpp>

namespace nate::Modules::Render
{
    struct Material
    {
        Vector3<float> Ambient;
        Vector3<float> Diffuse;
        Vector3<float> Specular;

        float Shininess;
    };
} // namespace nate::Modules::Render
