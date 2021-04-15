#pragma once

#include <BetaRay/Hittables/IHittable.hpp>
#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Hittables
{

    class MovingSphere : public IHittable
    {
    public:
        using MaterialPtr = Materials::IMaterial::shared_ptr;

        Point Center0;
        Point Center1;
        Scalar Time0;
        Scalar Time1;
        Scalar Radius;
        MaterialPtr Material;

        MovingSphere(
            Point const & center0,
            Point const & center1,
            Scalar time0,
            Scalar time1,
            Scalar radius,
            MaterialPtr material
        )
            : Center0(center0)
            , Center1(center1)
            , Time0(time0)
            , Time1(time1)
            , Radius(radius)
            , Material(std::move(material))
        { }

        Point Center(Scalar time) const
        {
            assert(Time0 <= time && time <= Time1);
            return Center0 + ((time - Time0) / (Time1 - Time0)) * (Center1 - Center0);
        }

        HitResultOpt Hit(Ray const & ray, Scalar tMin, Scalar tMax) const override
        {
            auto oc = ray.Origin - Center(ray.Time);
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

            auto intersect  = root;
            auto point      = ray.At(intersect);
            auto uv         = UV(point, ray.Time);
            auto normal     = (point - Center(ray.Time)) / Radius;
            bool frontFace  = glm::dot(ray.Direction, normal) < 0.0;
            normal          = frontFace ? normal : -normal;

            return HitResult { point, normal, uv, intersect, Material, frontFace };
        }

        BoundingBox Bounds(Scalar time0, Scalar time1) const override
        {
            auto radius = std::abs(Radius);

            return BoundingBox::Surround(
                BoundingBox(Center(time0) - Vec3(radius), Center(time0) + Vec3(radius)),
                BoundingBox(Center(time1) - Vec3(radius), Center(time1) + Vec3(radius)));
        }

        Vec2 UV(Point const & point, Scalar time) const
        {
            auto p = glm::normalize(point - Center(time));

            auto theta = glm::acos(-p.y);
            auto phi = glm::atan(-p.z, p.x) + glm::pi<Scalar>();

            return Vec2(
                phi / glm::two_pi<Scalar>(),
                theta / glm::pi<Scalar>());
        }
    };

}