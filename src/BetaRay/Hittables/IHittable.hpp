#pragma once

#include <BetaRay/Hittables/BoundingBox.hpp>
#include <BetaRay/Hittables/HitResult.hpp>


namespace BetaRay::Hittables
{

    class IHittable
    {
    public:
        using shared_ptr = std::shared_ptr<IHittable const>;

        virtual HitResultOpt Hit(Ray const & ray, Scalar tMin, Scalar tMax) const = 0;
        virtual BoundingBox Bounds(Scalar time0, Scalar time1) const = 0;
    };

}