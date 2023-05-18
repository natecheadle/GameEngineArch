#ifndef LIGHT_DIRECTIONAL_FRAG
#define LIGHT_DIRECTIONAL_FRAG

#include "Light.frag"
#include "Material.frag"

struct Light_Directional
{
    vec3 Direction;

    Light Light;
};

// calculates the color when using a directional light.
vec3 Calculate_Light_Directional(Light_Directional light, vec3 normal, vec3 viewDir, Material material, vec2 texCoords)
{
    vec3  lightDir   = normalize(-light.Direction);
    // diffuse shading
    float diff       = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3  reflectDir = reflect(-lightDir, normal);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    // combine results

    return Calculate_Light(light.Light, diff, spec, 1.0, material, texCoords);
}

#endif
