#include <BetaRay/Camera.hpp>
#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Hittables/HittableList.hpp>
#include <BetaRay/Hittables/Sphere.hpp>
#include <BetaRay/Materials/Dielectric.hpp>
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
        auto scatterResult = hit.Material->Scatter(ray, hit.Point, hit.Normal, hit.FrontFace);

        if (scatterResult.Scattered.has_value())
            return scatterResult.Attenuation * RayColor(scatterResult.Scattered.value(), scene, depth - 1);
        return Colors::Black;
    }

    auto d = glm::clamp(ray.Direction, -1.0, 1.0);
    auto t = 0.5 * (ray.Direction.y + 1.0);
    return (1.0 - t) * Colors::White + t * Color(0.5, 0.7, 1.0);
}


std::unique_ptr<IHittable> RandomScene()
{
    auto scene = std::make_unique<HittableList>();

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.6, 0.4));
    scene->Objects.emplace_back(std::make_shared<Sphere>(Point(0, -1000, 0), 1000, groundMaterial));

    std::uniform_real_distribution dist(0.0, 1.0);
    std::mt19937 gen;

    auto random = [&]() { return dist(gen); };

    for (auto a = -11; a < 11; ++a)
    {
        for (auto b = -11; b < 11; ++b)
        {
            auto matRand = random();
            auto position = Point(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if (glm::length(position - Point(4, 0.2, 0)) > 0.9)
            {
                if (matRand < 0.6)
                {
                    // diffuse
                    auto albedo = Color(random(), random(), random()) * Color(random(), random(), random());
                    auto material = std::make_shared<Lambertian>(albedo);
                    scene->Objects.emplace_back(std::make_shared<Sphere>(position, 0.2, material));
                }
                else if (matRand < 0.85)
                {
                    // metal
                    auto albedo = Color(0.5) + 0.5 * Color(random(), random(), random());
                    auto material = std::make_shared<Metal>(albedo, random() * 0.5);
                    scene->Objects.emplace_back(std::make_shared<Sphere>(position, 0.2, material));
                }
                else
                {
                    // glass
                    auto material = std::make_shared<Dielectric>(1.5);
                    scene->Objects.emplace_back(std::make_shared<Sphere>(position, 0.2, material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    scene->Objects.emplace_back(std::make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene->Objects.emplace_back(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene->Objects.emplace_back(std::make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    return scene;
}


int main()
{
    // Image
    Image image(3840u, 2160u);
    auto const samplesPerPixel = 60u;

    // Camera
    auto from           = Point(13.0, 3.0, 3.0);
    auto to             = Point( 0.0, 0.2, 0.0);
    auto up             = Point( 0.0, 1.0, 0.0);
    auto vfov           = 20.0;
    auto aperture       = 0.1;
    auto focalDistance  = 10.0; //glm::length(from - to);

    Camera camera(from, to, up, vfov, image.AspectRatio, aperture, focalDistance);

    // Scene
    auto scene = RandomScene();

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

                color += RayColor(ray, *scene);
            }

            // Gamma correction
            auto scale = 1.0 / samplesPerPixel;
            color = Color(glm::sqrt(color.r * scale), glm::sqrt(color.g * scale), glm::sqrt(color.b * scale));

            image.WritePixel(x, y, color);

            meter.Set(y);
        }
    }

    image.Save("img14.png");

    return 0;
}