#pragma once

#include <BetaRay/Materials/ScatterResult.hpp>


namespace BetaRay::Materials
{

    class IMaterial
    {
    public:
        using shared_ptr = std::shared_ptr<IMaterial const>;

        virtual ScatterResult Scatter(
            Ray const & ray,
            Point const & point,
            Vec const & normal,
            bool frontFace) const = 0;
    };

}