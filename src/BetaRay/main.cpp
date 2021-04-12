#include <BetaRay/Camera.hpp>
#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Hittables/HittableList.hpp>
#include <BetaRay/Hittables/Sphere.hpp>
#include <BetaRay/Materials/Lambertian.hpp>
#include <BetaRay/Materials/Metal.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Hittables;
using namespace BetaRay::Materials;
using namespace BetaRay::Utils;


Color RayColor(Ray const & ray, IHittable const & scene, u32 depth = 256)
{
    if (depth <= 0)
        return Colors::Black;

    auto hitResult = scene.Hit(ray, 1e-4, std::numeric_limits<Scalar>::infinity());
    if (hitResult.has_value())
    {
        auto hit = hitResult.value();
        auto scatterResult = hit.Material->Scatter(ray, hit.Point, hit.Normal);

        if (scatterResult.Scattered.has_value())
            return scatterResult.Attenuation * RayColor(scatterResult.Scattered.value(), scene, depth - 1);
        return Colors::Black;
    }

    auto d = glm::clamp(ray.Direction, -1.0, 1.0);
    auto t = 0.5 * (ray.Direction.y + 1.0);
    return (1.0 - t) * Colors::White + t * Color(0.5, 0.7, 1.0);
}


int main()
{
    // Image
    Image image(1920u, 1024u);
    auto const samplesPerPixel = 40u;

    // Camera
    Camera camera(image.AspectRatio);

    // Scene
    auto ground = std::make_shared<Lambertian>(Color(0.7, 0.8, 0.4));
    auto center = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto left   = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    auto scene = HittableList();
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point( 0.0, -100.5, -1.0), 100.0, ground));
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point( 0.0,    0.0, -1.0),   0.5, center));
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point(-1.0,    0.0, -1.0),   0.5, left));
    scene.Objects.emplace_back(std::make_shared<Sphere>(Point( 1.0,    0.0, -1.0),   0.5, right));

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

    image.Save("img4.png");

    return 0;
}