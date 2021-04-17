#pragma once

#include <BetaRay/Noise/INoise.hpp>
#include <BetaRay/Textures/ITexture.hpp>


namespace BetaRay::Textures
{

    class NoiseTexture : public ITexture
    {
    public:
        using NoisePtr = Noise::INoise::shared_ptr;

        NoisePtr Noise;
        Scalar Frequency;

        NoiseTexture(NoisePtr noise, Scalar frequency = 1.0)
            : Noise(std::move(noise))
            , Frequency(frequency)
        { }

        Color Value(Vec2 const & uv, Point const & point) const override
        {
            return Color(1.0) * 0.5 * (1.0 + Noise->Noise(Frequency * point));
        }
    };

}