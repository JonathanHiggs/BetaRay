#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay::Materials
{

    struct ScatterResult
    {
        Color Attenuation;
        std::optional<Ray> Scattered;

        ScatterResult(Color const & attenuation)
            : Attenuation(attenuation), Scattered(std::nullopt)
        { }

        ScatterResult(Color const & attenuation, Ray const & scattered)
            : Attenuation(attenuation), Scattered(scattered)
        { }
    };

}