#pragma once

#include <BetaRay/Hittables/IHittable.hpp>
#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Hittables
{

    class Sphere : public IHittable
    {
    public:
        using MaterialPtr = Materials::IMaterial::shared_ptr;

        Point Center;
        Scalar Radius;
        MaterialPtr Material;

        Sphere(Point const & center, Scalar radius, MaterialPtr material)
            : Center(center), Radius(radius), Material(std::move(material))
        { }

        std::optional<HitResult> Hit(Ray const & ray, Scalar tMin, Scalar tMax) const override
        {
            auto oc = ray.Origin - Center;
            auto a = glm::dot(ray.Direction, ray.Direction);
            auto halfB = glm::dot(oc, ray.Direction);
            auto c = glm::dot(oc, oc) - Radius * Radius;
            auto discriminant = halfB * halfB - a * c;

            if (discriminant < 0.0)
                return std::nullopt;

            auto sqrtDisc = glm::sqrt(discriminant);

            // Check: root is in ray direction units, not absolute distance
            auto root = (- halfB - sqrtDisc) / a;
            if (root < tMin || root > tMax)
            {
                root = (-halfB + sqrtDisc) / a;
                if (root < tMin || root > tMax)
                    return std::nullopt;
            }

            auto intersect = root;
            auto point = ray.At(intersect);
            auto normal = (point - Center) / Radius;
            bool frontFace = glm::dot(ray.Direction, normal) < 0.0;

            return HitResult {
                point,
                frontFace ? normal : -normal,
                intersect,
                Material,
                frontFace };
        }
    };

}