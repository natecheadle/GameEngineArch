#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    sampler2D Normal;
    sampler2D Height;
    float     Shininess;
};

#endif
