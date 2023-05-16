#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm_mat;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    Normal      = norm_mat * aNorm;
    TexCoords   = aTexCoords;
    FragPos     = vec3(model * vec4(aPos, 1.0));
}
