#pragma once

#include <BetaRay/Hittables/IHittable.hpp>
#include <BetaRay/Camera.hpp>


namespace BetaRay
{

    struct Scene
    {
        Hittables::IHittable::unique_ptr Hittables;
        Camera Camera;
    };

}