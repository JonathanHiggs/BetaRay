#pragma once

#include <BetaRay/Utils/includes.hpp>


namespace BetaRay::Noise
{

    class INoise
    {
    public:
        using shared_ptr = std::shared_ptr<INoise>;

        virtual Scalar Noise(Point const & point) const = 0;
    };

    Scalar TrilinearInterp(Scalar vals[2][2][2], Scalar u, Scalar v, Scalar w)
    {
        auto accum = Scalar(0.0);

        for (auto i = 0u; i < 2u; ++i)
        {
            for (auto j = 0u; j < 2u; ++j)
            {
                for (auto k = 0u; k < 2u; ++k)
                {
                    accum +=
                          (i * u + (1 - i) * (1 - u))
                        * (j * v + (1 - j) * (1 - v))
                        * (k * w + (1 - k) * (1 - w))
                        * vals[i][j][k];
                }
            }
        }

        return accum;
    }

    Scalar PerlinInterp(Vec3 vals[2][2][2], Scalar u, Scalar v, Scalar w)
    {
        auto uu  = u * u * (3.0 - 2.0 * u);
        auto vv  = v * v * (3.0 - 2.0 * v);
        auto ww  = w * w * (3.0 - 2.0 * w);
        auto accum = Scalar(0.0);

        for (auto i = 0u; i < 2u; ++i)
        {
            for (auto j = 0u; j < 2u; ++j)
            {
                for (auto k = 0u; k < 2u; ++k)
                {
                    auto weight = Vec3(u - i, v - j, w - k);
                    accum +=
                          (i * uu + (1 - i) * (1 - uu))
                        * (j * vv + (1 - j) * (1 - vv))
                        * (k * ww + (1 - k) * (1 - ww))
                        * glm::dot(vals[i][j][k], weight);
                }
            }
        }

        return accum;
    }

}