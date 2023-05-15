#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNorm;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norm_mat;

out vec3 ourColor;
out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    ourColor    = aColor;
    Normal      = norm_mat * aNorm;
    TexCoord    = aTexCoord;
    FragPos     = vec3(model * vec4(aPos, 1.0));
}
