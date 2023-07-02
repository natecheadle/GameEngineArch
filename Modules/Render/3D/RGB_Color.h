#pragma once

#include "Color.h"

namespace Ignosi::Modules::Render
{
    class RGB_Color : public Color
    {
      public:
        RGB_Color() noexcept = default;
        RGB_Color(float red, float green, float blue) noexcept
            : Color({red, green, blue})
        {
        }

        RGB_Color(Vector3<float> val) noexcept
            : Color(std::move(val))
        {
        }

        float Red() const { return Data()[0]; }
        float Green() const { return Data()[1]; }
        float Blue() const { return Data()[2]; }

        void Red(float val) { Data()[0] = val; }
        void Green(float val) { Data()[1] = val; }
        void Blue(float val) { Data()[2] = val; }
    };
} // namespace Ignosi::Modules::Render
