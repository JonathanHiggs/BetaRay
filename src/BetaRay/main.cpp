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
    auto const samplesPerPixel = 24u;

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

            color = color / Scalar(samplesPerPixel);

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img.png");

    return 0;
}