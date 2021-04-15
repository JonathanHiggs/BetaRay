#pragma once

#include <BetaRay/Utils/includes.hpp>


namespace BetaRay
{

    class Ray
    {
    public:
        Point Origin;
        Vec3 Direction;
        Scalar Time;

        Ray(Point const & origin, Vec3 const & direction, Scalar time)
            : Origin(origin), Direction(direction), Time(time)
        { }

        Point At(Distance distance) const { return Origin + distance * Direction; }
    };

}