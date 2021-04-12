#pragma once

#include <BetaRay/Hittables/HitResult.hpp>


namespace BetaRay::Hittables
{

    class IHittable
    {
    public:
        using shared_ptr = std::shared_ptr<IHittable const>;

        virtual std::optional<HitResult> Hit(Ray const & ray, Scalar tMin, Scalar tMax) const = 0;
    };

}