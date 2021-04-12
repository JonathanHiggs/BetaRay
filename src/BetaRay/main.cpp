#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Hittables/HittableList.hpp>
#include <BetaRay/Hittables/Sphere.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Hittables;
using namespace BetaRay::Utils;


Color RayColor(Ray const & ray, IHittable const & scene)
{
    auto result = scene.Hit(ray, 0.0, std::numeric_limits<Scalar>::infinity());
    if (result.has_value())
    {
        auto normal = result.value().Normal;
        return 0.5 * (normal + Vec(1.0));
    }

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

    // Scene
    auto scene = HittableList();
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point(0, 0, -1), 0.5));
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point(0, -100.5, -1), 100));

    // Render
    ProgressMeter meter(image.Height);

    for (auto y = 0u; y < image.Height; ++y)
    {
        for (auto x = 0u; x < image.Width; ++x)
        {
            auto u = double(x) / (image.Width - 1u);
            auto v = double(y) / (image.Height - 1u);

            auto ray = Ray(origin, lowerLeft + u * horizontal + v * vertical - origin);

            auto color = RayColor(ray, scene);

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img.png");

    return 0;
}