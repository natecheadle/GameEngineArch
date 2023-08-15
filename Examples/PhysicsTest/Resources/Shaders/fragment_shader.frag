#version 330 core

#include "Material.frag"

in vec2  TexCoords;
out vec4 color;

uniform Material material;
uniform vec3     spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(material.Diffuse, TexCoords);
}