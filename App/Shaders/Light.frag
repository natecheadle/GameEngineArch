#ifndef LIGHT_FRAG
#define LIGHT_FRAG

#include "Material.frag"

struct Light
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

vec3 Calculate_Light(Light light, float diff, float spec, float attenuation, Material material, vec2 texCoords)
{
    vec3 ambient  = light.Ambient * vec3(texture(material.Diffuse, texCoords));
    vec3 diffuse  = light.Diffuse * diff * vec3(texture(material.Diffuse, texCoords));
    vec3 specular = light.Specular * spec * vec3(texture(material.Specular, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

#endif
