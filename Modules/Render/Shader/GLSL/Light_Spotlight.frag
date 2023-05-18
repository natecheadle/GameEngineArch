#ifndef LIGHT_SPOTLIGHT_FRAG
#define LIGHT_SPOTLIGHT_FRAG

#include "Light.frag"
#include "Light_Attenuation.frag"
#include "Material.frag"

struct Light_SpotLight
{
    vec3        Position;
    vec3        Direction;
    float       CutOff;
    float       OuterCutOff;
    Attenuation Attenuation;
    Light       Light;
};

// calculates the color when using a spot light.
vec3 Calculate_Light_SpotLight(
    Light_SpotLight light,
    vec3            normal,
    vec3            fragPos,
    vec3            viewDir,
    Material        material,
    vec2            texCoords)
{
    vec3  lightDir    = normalize(light.Position - fragPos);
    // diffuse shading
    float diff        = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3  reflectDir  = reflect(-lightDir, normal);
    float spec        = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    // attenuation
    float attenuation = Calculate_Light_Attenuation(light.Attenuation, light.Position, fragPos);
    // spotlight intensity
    float theta       = dot(lightDir, normalize(-light.Direction));
    float epsilon     = light.CutOff - light.OuterCutOff;
    float intensity   = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // combine results

    return Calculate_Light(light.Light, diff, spec, attenuation, material, texCoords) * intensity;
}

#endif
