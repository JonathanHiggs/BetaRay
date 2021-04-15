#pragma once

#include <BetaRay/Materials/IMaterial.hpp>
#include <BetaRay/Textures/ITexture.hpp>
#include <BetaRay/Textures/SolidColor.hpp>


namespace BetaRay::Materials
{

    class Lambertian : public IMaterial
    {
    public:
        Textures::ITexture::shared_ptr Albedo;

        Lambertian(Color const & albedo) : Albedo(std::make_shared<Textures::SolidColor>(albedo)) { }

        Lambertian(Textures::ITexture::shared_ptr albedo) : Albedo(std::move(albedo)) { }

        ScatterResult Scatter(
            Ray const & ray,
            Point const & point,
            Vec3 const & normal,
            Vec2 const & uv,
            bool frontFace) const override
        {
            auto attenuation = Albedo->Value(uv, point);

            // ToDo: pass generator though
            auto direction = glm::sphericalRand(1.0);
            direction = glm::dot(direction, normal) > 0.0 ? direction : -direction;
            auto scattered = Ray(point, normal + direction, ray.Time);

            return ScatterResult(attenuation, scattered);
        }
    };

}