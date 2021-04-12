#pragma once

#include <BetaRay/Utils/includes.hpp>


namespace BetaRay
{

    class Ray
    {
    public:
        Point Origin;
        Vec Direction;

        Ray(Point const & origin, Vec const & direction)
            : Origin(origin), Direction(direction)
        { }

        Point At(Distance distance) const { return Origin + distance * Direction; }
    };

}