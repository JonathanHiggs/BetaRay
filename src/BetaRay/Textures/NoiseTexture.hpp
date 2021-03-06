#pragma once

#include <BetaRay/Noise/INoise.hpp>
#include <BetaRay/Textures/ITexture.hpp>


namespace BetaRay::Textures
{

    class PhaseAdjustedNoiseTexture : public ITexture
    {
    public:
        using NoisePtr = Noise::INoise::shared_ptr;

        NoisePtr Noise;
        Scalar Frequency;
        Scalar Gate;

        PhaseAdjustedNoiseTexture(NoisePtr noise, Scalar frequency = 10.0, Scalar gate = 2.0)
            : Noise(std::move(noise))
            , Frequency(frequency)
            , Gate(gate)
        { }

        Color Value(Vec2 const & uv, Point const & point) const override
        {
            auto value = glm::sin(Gate * point.z + Frequency * Noise->Noise(point));
            return Color(1.0) * 0.5 * (1.0 + value);
        }
    };

}