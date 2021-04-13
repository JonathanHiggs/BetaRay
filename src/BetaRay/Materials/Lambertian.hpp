#pragma once

#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Materials
{

    class Lambertian : public IMaterial
    {
    public:
        Color Albedo;

        Lambertian(Color const & albedo) : Albedo(albedo) { }

        ScatterResult Scatter(
            Ray const & ray,
            Point const & point,
            Vec const & normal,
            bool frontFace) const override
        {
            auto attenuation = Albedo;

            // ToDo: pass generator though
            auto direction = glm::sphericalRand(1.0);
            direction = glm::dot(direction, normal) > 0.0 ? direction : -direction;
            auto scattered = Ray(point, normal + direction, ray.Time);

            return ScatterResult(attenuation, scattered);
        }
    };

}