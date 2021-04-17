#pragma once

#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Textures/ITexture.hpp>


namespace BetaRay::Textures
{

    class ImageTexture : public ITexture
    {
    public:
        using ImagePtr = Files::Image::shared_ptr;

        ImagePtr Image;

        ImageTexture(ImagePtr image) : Image(std::move(image)) { }

        Color Value(Vec2 const & uv, Point const & point) const override
        {
            auto u = glm::clamp(uv.x, 0.0, 1.0);
            auto v = glm::clamp(uv.y, 0.0, 1.0);

            auto x = static_cast<Files::Image::size_type>(u * Image->Width);
            auto y = static_cast<Files::Image::size_type>(v * Image->Height);

            return Image->ReadPixel(x, y);
        }
    };

}