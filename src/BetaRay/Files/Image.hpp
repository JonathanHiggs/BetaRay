#pragma once

#include <BetaRay/Utils/includes.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


namespace BetaRay::Files
{

    struct Pixel
    {
        u8 r = 0u;
        u8 g = 0u;
        u8 b = 0u;
        u8 a = 255u;
    };

    class Image
    {
    public:
        using size_type = u32;
        constexpr static size_type Channels = 4u;

        size_type Width;
        size_type Height;
        Scalar AspectRatio;
        std::vector<Pixel> Data;

        Image(size_type width, size_type height)
            : Width(width)
            , Height(height)
            , AspectRatio(Scalar(Width) / Scalar(Height))
        {
            Data.resize(Width * Height);
        }

        void WritePixel(size_type x, size_type y, Color color)
        {
            auto index = y * Width + x;

            if (index > Data.size())
            {
                assert(false);
                return;
            }

            Data[index] = Pixel {
                static_cast<u8>(255.999 * glm::clamp(color.r, Scalar(0.0), Scalar(1.0))),
                static_cast<u8>(255.999 * glm::clamp(color.g, Scalar(0.0), Scalar(1.0))),
                static_cast<u8>(255.999 * glm::clamp(color.b, Scalar(0.0), Scalar(1.0))) };
        }

        void Save(std::string const & path)
        {
            stbi_flip_vertically_on_write(1);

            auto const stride = Width * Channels;
            stbi_write_png(path.c_str(), Width, Height, Channels, Data.data(), stride);
        }
    };

}