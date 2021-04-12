#pragma once

#include <BetaRay/Utils/includes.hpp>

#include <windows.h>


namespace BetaRay::Utils
{

    u32 ConsoleWidth()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        return info.srWindow.Right - info.srWindow.Left + 1;
    }

    class ProgressMeter
    {
    public:
        u32 Lines;
        u32 Width;
        u32 Progress = 0u;

        ProgressMeter(u32 lines)
            : Lines(lines)
            , Width(glm::clamp(ConsoleWidth(), 20u, 200u))
        {
            std::cout << '|' << std::string(Width - 2, '-') << "|\n";
        }

        void Set(u32 line)
        {
            auto progress = static_cast<u32>(Scalar(line + 1) * Width / Lines);

            while (Progress < progress)
            {
                std::cout << '#';
                ++Progress;
            }
        }
    };

}