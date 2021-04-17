#pragma once

#include <BetaRay/Noise/Perlin.hpp>
#include <BetaRay/Textures/ITexture.hpp>


namespace BetaRay::Textures
{

    class NoiseTexture : public ITexture
    {
    public:
        Noise::Perlin PerlinNoise;
        Scalar Frequency;

        NoiseTexture(Scalar frequency = 1.0) : PerlinNoise(), Frequency(frequency) { }

        Color Value(Vec2 const & uv, Point const & point) const override
        {
            return Color(1.0) * 0.5 * (1.0 + PerlinNoise.Noise(Frequency * point));
            //return Color(1.0) * 0.5 * (1.0 + glm::perlin(Frequency * point));
        }
    };

}