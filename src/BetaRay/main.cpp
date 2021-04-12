#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Utils;


Color RayColor(Ray const & ray)
{
    auto d = glm::clamp(ray.Direction, -1.0, 1.0);
    auto t = 0.5 * (ray.Direction.y + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // Image
    Image image(480u, 360u);

    // Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = image.AspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point();
    auto horizontal = Vec(viewportWidth, 0.0, 0.0);
    auto vertical = Vec(0.0, viewportHeight, 0.0);
    auto lowerLeft = origin - horizontal / 2.0 - vertical / 2.0 - Vec(0.0, 0.0, focalLength);

    // Render
    ProgressMeter meter(image.Height);

    for (auto y = 0u; y < image.Height; ++y)
    {
        for (auto x = 0u; x < image.Width; ++x)
        {
            auto u = double(x) / (image.Width - 1u);
            auto v = double(y) / (image.Height - 1u);

            auto ray = Ray(origin, lowerLeft + u * horizontal + v * vertical - origin);

            auto color = RayColor(ray);

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img.png");

    return 0;
}