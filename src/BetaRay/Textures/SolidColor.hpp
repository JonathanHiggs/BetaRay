#pragma once

#include <BetaRay/Textures/ITexture.hpp>


namespace BetaRay::Textures
{

    class SolidColor : public ITexture
    {
    public:
        Color ColorValue;

        SolidColor(Color color) : ColorValue(color) {}

        Color Value(Vec2 const & uv, Point const & point) const override { return ColorValue; }
    };

}