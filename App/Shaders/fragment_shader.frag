#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    vec4  tex             = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    float ambientStrength = 0.1;
    vec3  ambient         = ambientStrength * lightColor;

    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3  viewDir          = normalize(viewPos - FragPos);
    vec3  reflectDir       = reflect(-lightDir, norm);
    float spec             = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3  specular         = specularStrength * spec * lightColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0) * tex;
}
