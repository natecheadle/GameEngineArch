#ifndef LIGHT_ATTENUATION_FRAG
#define LIGHT_ATTENUATION_FRAG

struct Attenuation
{
    float Constant;
    float Linear;
    float Quadratic;
};

float Calculate_Light_Attenuation(Attenuation attenuation, vec3 lightPos, vec3 fragPos)
{
    float distance = length(lightPos - fragPos);
    return 1.0 / (attenuation.Constant + attenuation.Linear * distance + attenuation.Quadratic * (distance * distance));
}

#endif
