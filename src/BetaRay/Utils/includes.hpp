#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>


namespace BetaRay
{

    using i8 = int8_t;
    using i32 = int32_t;

    using u8 = uint8_t;
    using u32 = uint32_t;

    using r32 = float;
    using r64 = double;

    using Scalar = r64;
    using Vec = glm::dvec3;

    using Distance = Scalar;
    using Point = Vec;
    using Color = Vec;


    namespace Colors
    {

        constexpr Color Black   = Color(0.0);
        constexpr Color White   = Color(1.0);
        constexpr Color Red     = Color(1.0, 0.0, 0.0);
        constexpr Color Green   = Color(0.0, 1.0, 0.0);
        constexpr Color Blue    = Color(0.0, 0.0, 1.0);

    }

}