#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay::Hittables
{

    struct HitResult
    {
        Point Point;
        Vec Normal;
        Scalar Intersect;
    };

}