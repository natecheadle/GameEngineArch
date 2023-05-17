#ifndef MATERIAL_FRAG
#define MATERIAL_FRAG

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float     Shininess;
};

#endif
