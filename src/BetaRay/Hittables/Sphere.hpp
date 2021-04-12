#pragma once

#include <BetaRay/Ray.hpp>
#include <BetaRay/Hittables/HitResult.hpp>


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

        std::optional<HitResult> Hit(Ray const & ray) const
        {
            auto oc = ray.Origin - Center;
            auto a = glm::dot(ray.Direction, ray.Direction);
            auto halfB = glm::dot(oc, ray.Direction);
            auto c = glm::dot(oc, oc) - Radius * Radius;
            auto discriminant = halfB * halfB - a * c;

            if (discriminant < 0.0)
                return std::nullopt;

            auto intersect = (-halfB - glm::sqrt(discriminant)) / a;
            auto point = ray.At(intersect);
            auto normal = (point - Center) / Radius;

            return HitResult { point, normal, intersect };
        }
    };

}