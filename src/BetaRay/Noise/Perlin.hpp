#pragma once

#include <BetaRay/Noise/INoise.hpp>


namespace BetaRay::Noise
{

    class Perlin : public INoise
    {
    public:
        Perlin()
        {
            std::uniform_real_distribution dist(-1.0, 1.0);
            std::mt19937 gen;

            std::generate(std::begin(randoms), std::end(randoms), [&]() {
                return Vec3(dist(gen), dist(gen), dist(gen)); });

            Generate(std::begin(permX), std::end(permX), 1);
            Generate(std::begin(permY), std::end(permY), 2);
            Generate(std::begin(permZ), std::end(permZ), 3);
        }

        Scalar Noise(Point const & point) const override
        {
            auto u = point.x - std::floor(point.x);
            auto v = point.y - std::floor(point.y);
            auto w = point.z - std::floor(point.z);

            // u  = u * u * (3.0 - 2.0 * u);
            // v  = v * v * (3.0 - 2.0 * v);
            // w  = w * w * (3.0 - 2.0 * w);

            auto i = static_cast<u32>(std::floor(point.x));
            auto j = static_cast<u32>(std::floor(point.y));
            auto k = static_cast<u32>(std::floor(point.z));
            Vec3 c[2][2][2];

            for (auto di = 0u; di < 2u; ++di)
            {
                for (auto dj = 0u; dj < 2u; ++dj)
                {
                    for (auto dk = 0u; dk < 2u; ++dk)
                    {
                        c[di][dj][dk] = randoms[
                            permX[(i + di) & 255]
                            ^ permY[(j + dj) & 255]
                            ^ permZ[(k + dk) & 255] ];
                    }
                }
            }

            return PerlinInterp(c, u, v, w);
        }

    private:
        template<class _It>
        void Generate(_It first, _It last, i32 seed)
        {
            std::iota(first, last, 0);
            std::shuffle(first, last, std::default_random_engine(seed));
        }

        static constexpr size_t numberPoints = 256u;
        std::array<Vec3, numberPoints> randoms;
        std::array<i32, numberPoints> permX;
        std::array<i32, numberPoints> permY;
        std::array<i32, numberPoints> permZ;
    };

}