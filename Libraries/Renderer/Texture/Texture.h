#pragma once

#include "ITexture.h"

namespace Ignosi::Libraries::Renderer
{
    class Texture : public ITexture
    {
      private:
        const enum TextureUnit m_Unit;

      protected:
        Texture(enum TextureUnit unit);

      public:
        virtual ~Texture() = default;

        enum TextureUnit TextureUnit() const override;
    };
} // namespace Ignosi::Libraries::Renderer
