#version 330 core

struct Material
{
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3     viewPos;
uniform Material material;
uniform Light    light;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(light.position - FragPos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * material.diffuse);

    // specular
    vec3  viewDir    = normalize(viewPos - FragPos);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular   = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    vec4 color  = vec4(result, 1.0);

    // linearly interpolate between both textures (80% container, 20% awesomeface)
    vec4 tex  = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor = color * tex;
}
