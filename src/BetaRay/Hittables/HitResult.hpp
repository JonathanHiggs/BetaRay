#pragma once

#include <BetaRay/Ray.hpp>
#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Hittables
{

    struct HitResult
    {
        using MaterialPtr = Materials::IMaterial::shared_ptr;

        Point Point;
        Vec3 Normal;
        Vec2 UV;
        Scalar Intersect;
        MaterialPtr Material;
        bool FrontFace;
    };

    // ToDo: maybe rename: OptionalHitResult;
    using HitResultOpt = std::optional<HitResult>;

}