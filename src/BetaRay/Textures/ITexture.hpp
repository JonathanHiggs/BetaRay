#pragma once

#include <BetaRay/Utils/includes.hpp>


namespace BetaRay::Textures
{

    class ITexture
    {
    public:
        using shared_ptr = std::shared_ptr<ITexture>;

        virtual Color Value(Vec2 const & uv, Point const & point) const = 0;
    };

}