#pragma once

#include <BetaRay/Noise/INoise.hpp>
#include <BetaRay/Noise/Perlin.hpp>


namespace BetaRay::Noise
{

    class Turbulent : public INoise
    {
    public:
        shared_ptr InnerNoise;
        u32 Depth;

        Turbulent(shared_ptr innerNoise, u32 depth = 7u)
            : InnerNoise(std::move(innerNoise))
            , Depth(depth)
        { }

        Scalar Noise(Point const & point) const override
        {
            auto accum = Scalar(0);
            auto tempP = point;
            auto weight = 1.0;

            for (auto i = 0u; i < Depth; ++i)
            {
                accum += weight * InnerNoise->Noise(tempP);
                weight *= 0.5;
                tempP *= 2.0;
            }

            return 2.0 * (std::abs(accum) - 0.5);
        }
    };

}