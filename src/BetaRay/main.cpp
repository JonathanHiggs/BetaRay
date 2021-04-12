#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Utils;


int main()
{
    Image image(480u, 360u);

    // Render
    ProgressMeter meter(image.Height);

    for (auto y = 0u; y < image.Height; ++y)
    {
        for (auto x = 0u; x < image.Width; ++x)
        {
            auto color = Color{
                Scalar(x) / image.Width,
                Scalar(y) / image.Height,
                0.5 };

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img.png");

    return 0;
}