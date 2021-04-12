#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay::Hittables
{

    struct HitResult
    {
        Point Point;
        Vec Normal;
        Scalar Intersect;
        bool FrontFace;
    };

    using HitResultOpt = std::optional<HitResult>;

}