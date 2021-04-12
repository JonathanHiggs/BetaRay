#include <BetaRay/Camera.hpp>
#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Hittables/HittableList.hpp>
#include <BetaRay/Hittables/Sphere.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Hittables;
using namespace BetaRay::Utils;


Color RayColor(Ray const & ray, IHittable const & scene, u32 depth = 256)
{
    if (depth <= 0)
        return Colors::White;

    auto result = scene.Hit(ray, 1e-4, std::numeric_limits<Scalar>::infinity());
    if (result.has_value())
    {
        auto & res = result.value();
        auto target = res.Point + res.Normal + glm::sphericalRand(1.0);
        auto newRay = Ray(res.Point, target - res.Point);
        return 0.5 * RayColor(newRay, scene, depth - 1);
    }

    auto d = glm::clamp(ray.Direction, -1.0, 1.0);
    auto t = 0.5 * (ray.Direction.y + 1.0);
    return (1.0 - t) * Colors::White + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // Image
    Image image(480u, 360u);
    auto const samplesPerPixel = 100u;

    // Camera
    Camera camera(image.AspectRatio);

    // Scene
    auto scene = HittableList();
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point(0, 0, -1), 0.5));
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point(0, -100.5, -1), 100));

    // Render
    ProgressMeter meter(image.Height);

    std::uniform_real_distribution<Scalar> distribution(0.0, 1.0);
    std::mt19937 generator;

    for (auto y = 0u; y < image.Height; ++y)
    {
        for (auto x = 0u; x < image.Width; ++x)
        {
            Color color(0.0);

            for (auto i = 0u; i < samplesPerPixel; ++i)
            {
                auto u = (Scalar(x) + distribution(generator)) / (image.Width - 1u);
                auto v = (Scalar(y) + distribution(generator)) / (image.Height - 1u);

                auto ray = camera.RayFromViewport(u, v);

                color += RayColor(ray, scene);
            }

            // Gamma correction
            auto scale = 1.0 / samplesPerPixel;
            color = Color(glm::sqrt(color.r * scale), glm::sqrt(color.g * scale), glm::sqrt(color.b * scale));

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img3.png");

    return 0;
}