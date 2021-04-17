#pragma once

#include <BetaRay/Utils/includes.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


namespace BetaRay::Files
{

    class Image
    {
    public:
        using size_type = u32;

        size_type Width;
        size_type Height;
        Scalar AspectRatio;
        size_type Channels;
        std::vector<u8> Data;

        Image(size_type width, size_type height)
            : Width(width)
            , Height(height)
            , AspectRatio(Scalar(Width) / Scalar(Height))
            , Channels(4u)
        {
            Data.resize(Width * Height * Channels);
        }

        Image(size_type width, size_type height, size_type channels, std::vector<u8> && data)
            : Width(width)
            , Height(height)
            , AspectRatio(Scalar(Width) / Scalar(Height))
            , Channels(channels)
            , Data(std::move(data))
        { }

        static std::unique_ptr<Image> LoadFromFile(std::string const & path)
        {
            std::call_once(loadFlipFlag, []() { stbi_set_flip_vertically_on_load(1); });

            i32 width, height, channels;
            u8 * data = stbi_load(path.c_str(), &width, &height, &channels, 0);

            std::vector<u8> data_;
            auto length = width * height * channels;
            data_.assign(data, data + length);

            if (channels < 3u || !data)
            {
                width = 1u;
                height = 1u;
                channels = 4u;
                data_ = std::vector<u8>({255, 0, 0, 255});
            }

            return std::make_unique<Image>(width, height, channels, std::move(data_));
        }

        Color ReadPixel(size_type x, size_type y) const
        {
            x = glm::clamp(x, 0u, Width);
            y = glm::clamp(y, 0u, Height);

            auto index = Channels * (y * Width + x);

            assert(index + Channels - 1 < Data.size());

            auto r = Scalar(Data[index + 0u]) / ColorScale;
            auto g = Scalar(Data[index + 1u]) / ColorScale;
            auto b = Scalar(Data[index + 2u]) / ColorScale;

            return Color(r, g, b);
        }

        void WritePixel(size_type x, size_type y, Color color)
        {
            auto index = Channels * (y * Width + x);

            assert(index + Channels - 1 < Data.size());

            Data[index + 0u] = static_cast<u8>(255.999 * glm::clamp(color.r, Scalar(0.0), Scalar(1.0)));
            Data[index + 1u] = static_cast<u8>(255.999 * glm::clamp(color.g, Scalar(0.0), Scalar(1.0)));
            Data[index + 2u] = static_cast<u8>(255.999 * glm::clamp(color.b, Scalar(0.0), Scalar(1.0)));

            if (Channels == 4)
                Data[index + 3u] = 255;
        }

        void Save(std::string const & path)
        {
            std::call_once(saveFlipFlag, []() { stbi_flip_vertically_on_write(1); });

            auto const stride = Width * Channels;
            stbi_write_png(path.c_str(), Width, Height, Channels, Data.data(), stride);
        }

    private:
        constexpr static Scalar ColorScale = 1.0 / 255.0;

        static std::once_flag loadFlipFlag;
        static std::once_flag saveFlipFlag;
    };

    std::once_flag Image::saveFlipFlag;
    std::once_flag Image::loadFlipFlag;

}