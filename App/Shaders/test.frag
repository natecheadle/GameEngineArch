#version 330 core

#ifndef LIGHT_DIRECTIONAL_FRAG
#define LIGHT_DIRECTIONAL_FRAG

#ifndef LIGHT_FRAG
#define LIGHT_FRAG

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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

#ifndef LIGHT_POINT_FRAG
#define LIGHT_POINT_FRAG

#ifndef LIGHT_FRAG
#define LIGHT_FRAG

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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

    return Calculate_Light(light.Light, spec, attenuation, material, texCoords);
}

#endif

#ifndef LIGHT_SPOTLIGHT_FRAG
#define LIGHT_SPOTLIGHT_FRAG

#ifndef LIGHT_FRAG
#define LIGHT_FRAG

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

struct Light_SpotLight
{
    vec3  Position;
    vec3  Direction;
    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    Light Light;
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
    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    // spotlight intensity
    float theta       = dot(lightDir, normalize(-light.Direction));
    float epsilon     = light.CutOff - light.OuterCutOff;
    float intensity   = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // combine results

    return Calculate_Light(light.Light, diff, spec, attenuation, material, texCoords) * intensity;
}

#endif

#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif

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