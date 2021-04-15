#pragma once

#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Materials
{

    class Metal : public IMaterial
    {
    public:
        Color Albedo;
        Scalar Fuzz;

        Metal(Color const & albedo, Scalar fuzz)
            : Albedo(albedo), Fuzz(fuzz)
        { }

        ScatterResult Scatter(
            Ray const & ray,
            Point const & point,
            Vec3 const & normal,
            Vec2 const & uv,
            bool frontFace) const override
        {
            auto attenuation = Albedo;

            auto reflected = glm::reflect(glm::normalize(ray.Direction), normal) + glm::sphericalRand(Fuzz);
            auto scatteredRay = Ray(point, reflected, ray.Time);
            auto scattered = glm::dot(scatteredRay.Direction, normal) > 0.0;

            if (scattered)
                return ScatterResult(attenuation, scatteredRay);
            else
                return ScatterResult(attenuation);
        }
    };

}