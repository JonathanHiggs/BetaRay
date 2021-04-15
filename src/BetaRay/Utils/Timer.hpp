#pragma once

#include <BetaRay/Utils/includes.hpp>


namespace BetaRay::Utils
{

    class Timer
    {
    private:
        std::ostream & os;
        std::string identifier;
        std::chrono::time_point<std::chrono::steady_clock> start;

    public:
        Timer(std::ostream & os, std::string identifier)
            : os(os)
            , identifier(std::move(identifier))
            , start(std::chrono::high_resolution_clock::now())
        { }

        ~Timer()
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = end - start;
            auto ms = duration.count() * 1e-6;

            os << identifier << " took: ";

            if (ms < 1100)
                os << ms << " ms\n";
            else if (ms < 61000)
                os << ms / 1000 << " s\n";
            else if (ms < 3660000)
                os << ms / 60000 << " m\n";
            else
                os << ms / 3600000 << " h\n";
        }
    };

}