#pragma once

#include <BetaRay/Textures/ITexture.hpp>
#include <BetaRay/Textures/SolidColor.hpp>


namespace BetaRay::Textures
{

    class CheckerTexture : public ITexture
    {
    public:
        shared_ptr Odd;
        shared_ptr Even;

        CheckerTexture(shared_ptr odd, shared_ptr even)
            : Odd(std::move(odd))
            , Even(std::move(even))
        { }

        CheckerTexture(Color odd, Color even)
            : Odd(std::make_shared<SolidColor>(odd))
            , Even(std::make_shared<SolidColor>(even))
        { }

        Color Value(Vec2 const & uv, Point const & point) const override
        {
            auto sines = glm::sin(10.0 * point.x) * glm::sin(10.0 * point.y) * glm::sin(10.0 * point.z);
            return sines < 0 ? Odd->Value(uv, point) : Even->Value(uv, point);
        }
    };

}