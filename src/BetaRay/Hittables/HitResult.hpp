#pragma once

#include <BetaRay/Ray.hpp>
#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Hittables
{

    struct HitResult
    {
        using MaterialPtr = Materials::IMaterial::shared_ptr;

        Point Point;
        Vec Normal;
        Scalar Intersect;
        MaterialPtr Material;
        bool FrontFace;
    };

    using HitResultOpt = std::optional<HitResult>;

}