#pragma once

#include <BetaRay/Materials/IMaterial.hpp>


namespace BetaRay::Materials
{

    class Dielectric : public IMaterial
    {
    public:
        Scalar RefractionIndex;

        Dielectric(Scalar refraction) : RefractionIndex(refraction) { }

        ScatterResult Scatter(
            Ray const & ray,
            Point const & point,
            Vec const & normal,
            bool frontFace) const override
        {
            // ToDo: hoist these out
            static std::uniform_real_distribution<Scalar> dist(0.0, 1.0);
            static std::mt19937 gen;

            auto attenuation = Colors::White;
            // Note: Assumes always Air <> material, not material <> material
            auto refractionRatio = frontFace ? (1.0 / RefractionIndex) : RefractionIndex;

            auto direction = glm::normalize(ray.Direction);
            auto cosTheta = glm::min(glm::dot(-direction, normal), 1.0);
            auto sinTheta = glm::sqrt(1.0 - cosTheta * cosTheta);

            auto cannotRefract = refractionRatio * sinTheta > 1.0;

            auto refracted = cannotRefract || Reflectance(cosTheta, RefractionIndex) > dist(gen)
                ? glm::reflect(direction, normal)
                : glm::refract(direction, normal, refractionRatio);

            auto scattered = Ray(point, refracted);

            return ScatterResult(attenuation, scattered);
        }

    private:
        static Scalar Reflectance(Scalar cosine, Scalar refractiveIndex)
        {
            // Schlick's approximation
            auto r0 = (1.0 - refractiveIndex) / (1.0 + refractiveIndex);
            r0 = r0 * r0;
            return r0 + (1.0 - r0) * glm::pow((1.0 - cosine), 5);
        }
    };

}