#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay::Hittables
{

    class Sphere
    {
    public:
        Point Center;
        Scalar Radius;

        Sphere(Point const & center, Scalar const & radius)
            : Center(center), Radius(radius)
        { }

        bool Hit(Ray const & ray) const
        {
            auto oc = ray.Origin - Center;
            auto a = glm::dot(ray.Direction, ray.Direction);
            auto b = 2.0 * glm::dot(oc, ray.Direction);
            auto c = glm::dot(oc, oc) - Radius * Radius;
            auto discriminant = b * b - 4.0 * a * c;

            return discriminant > 0.0;
        }
    };

}