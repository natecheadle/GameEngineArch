#ifndef LIGHT_POINT_FRAG
#define LIGHT_POINT_FRAG

#include "Light.frag"
#include "Material.frag"

struct Light_Point
{
    vec3 Position;

    float Constant;
    float Linear;
    float Quadratic;

    Light Light;
};

// calculates the color when using a point light.
vec3 Caluclate_Light_Point(
    Light_Point light,
    vec3        normal,
    vec3        fragPos,
    vec3        viewDir,
    Material    material,
    vec2        texCoords)
{
    vec3  lightDir    = normalize(light.Position - fragPos);
    // diffuse shading
    float diff        = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3  reflectDir  = reflect(-lightDir, normal);
    float spec        = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    // attenuation
    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    // combine results

    return Calculate_Light(light.Light, diff, spec, attenuation, material, texCoords);
}

#endif
