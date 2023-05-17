#version 330 core

#include "Light_Directional.frag"
#include "Light_Point.frag"
#include "Light_Spotlight.frag"
#include "Material.frag"

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3              viewPos;
uniform Light_Directional dirLight;
uniform Light_Point       pointLight;
uniform Light_SpotLight   spotLight;
uniform Material          material;

void main()
{
    // properties
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = Calculate_Light_Directional(dirLight, norm, viewDir, material, TexCoords);
    // phase 2: point lights
    result += Caluclate_Light_Point(pointLight, norm, FragPos, viewDir, material, TexCoords);
    // phase 3: spot light
    result += Calculate_Light_SpotLight(spotLight, norm, FragPos, viewDir, material, TexCoords);

    FragColor = vec4(result, 1.0);
}
