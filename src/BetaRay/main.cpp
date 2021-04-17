#include <BetaRay/Camera.hpp>
#include <BetaRay/Ray.hpp>
#include <BetaRay/Files/Image.hpp>
#include <BetaRay/Hittables/BvhNode.hpp>
#include <BetaRay/Hittables/HittableList.hpp>
#include <BetaRay/Hittables/MovingSphere.hpp>
#include <BetaRay/Hittables/Sphere.hpp>
#include <BetaRay/Materials/Dielectric.hpp>
#include <BetaRay/Materials/Lambertian.hpp>
#include <BetaRay/Materials/Metal.hpp>
#include <BetaRay/Noise/Perlin.hpp>
#include <BetaRay/Noise/Turbulent.hpp>
#include <BetaRay/Textures/CheckerTexture.hpp>
#include <BetaRay/Textures/NoiseTexture.hpp>
#include <BetaRay/Textures/PhaseAdjustedNoiseTexture.hpp>
#include <BetaRay/Utils/ProgressMeter.hpp>
#include <BetaRay/Utils/Timer.hpp>


using namespace BetaRay;
using namespace BetaRay::Files;
using namespace BetaRay::Hittables;
using namespace BetaRay::Materials;
using namespace BetaRay::Noise;
using namespace BetaRay::Textures;
using namespace BetaRay::Utils;


Color RayColor(Ray const & ray, IHittable const & scene, u32 depth = 256)
{
    if (depth <= 0)
        return Colors::Black;

    auto hitResult = scene.Hit(ray, 1e-4, std::numeric_limits<Scalar>::infinity());
    if (hitResult.has_value())
    {
        auto hit = hitResult.value();
        auto scatterResult = hit.Material->Scatter(ray, hit.Point, hit.Normal, hit.UV, hit.FrontFace);

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

    auto checkerTexture = std::make_shared<CheckerTexture>(Color(0.5, 0.6, 0.4), Color(0.9, 0.9, 0.9));
    auto groundMaterial = std::make_shared<Lambertian>(checkerTexture);
    scene->Objects.emplace_back(std::make_shared<Sphere>(Point(0, -1000, 0), 1000, groundMaterial));

    std::uniform_real_distribution dist(0.0, 1.0);
    std::mt19937 gen;

    auto random = [&]() { return dist(gen); };

    constexpr auto size = 11;
    for (auto a = -size; a < size; ++a)
    {
        for (auto b = -size; b < size; ++b)
        {
            auto matRand = random();
            auto center = Point(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if (glm::length(center - Point(4, 0.2, 0)) > 0.9)
            {
                if (matRand < 0.6)
                {
                    // diffuse
                    auto albedo = Color(random(), random(), random()) * Color(random(), random(), random());
                    auto material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0.0, dist(gen) * 0.1, 0.0);
                    scene->Objects.emplace_back(std::make_shared<MovingSphere>(
                        center, center2, 0.0, 1.0, 0.2, material));
                }
                else if (matRand < 0.85)
                {
                    // metal
                    auto albedo = Color(0.5) + 0.5 * Color(random(), random(), random());
                    auto material = std::make_shared<Metal>(albedo, random() * 0.5);
                    scene->Objects.emplace_back(std::make_shared<Sphere>(center, 0.2, material));
                }
                else
                {
                    // glass
                    auto material = std::make_shared<Dielectric>(1.5);
                    scene->Objects.emplace_back(std::make_shared<Sphere>(center, 0.2, material));
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

    //return scene;
    return std::make_unique<BvhNode>(*scene, 0.0, 0.3);
}


std::unique_ptr<IHittable> TwoPerlinSpheres()
{
    auto scene = std::make_unique<HittableList>();

    auto perlinNoise = std::make_shared<Perlin>();
    auto turbulentNoise = std::make_shared<Turbulent>(perlinNoise);

    auto phasedPerlinTexture = std::make_shared<PhaseAdjustedNoiseTexture>(turbulentNoise);
    auto turbulentTexture = std::make_shared<NoiseTexture>(turbulentNoise, 4.0);

    scene->Objects.emplace_back(
        std::make_shared<Sphere>(
            Point(0, -1000, 0), 1000, std::make_shared<Lambertian>(turbulentTexture)));

    scene->Objects.emplace_back(
        std::make_shared<Sphere>(
            Point(0, 2, 0), 2, std::make_shared<Lambertian>(phasedPerlinTexture)));

    return scene;
}


int main()
{
    Timer render(std::cout, "Rendering");

    // Image
    //Image image(3840u, 2160u);
    //Image image(1920u, 1024u);
    Image image(640u, 480u);
    auto const samplesPerPixel = 40u;

    // Camera
    auto from           = Point(13.0, 2.0, 3.0);
    auto to             = Point( 0.0, 0.0, 0.0);
    auto up             = Point( 0.0, 1.0, 0.0);
    auto vfov           = 20.0;
    auto aperture       = 0.01;
    auto focalDistance  = glm::length(from - to);
    auto time0          = 0.0;
    auto time1          = 0.3;

    Camera camera(from, to, up, vfov, image.AspectRatio, aperture, focalDistance, time0, time1);

    // Scene
    //auto scene = RandomScene();
    auto scene = TwoPerlinSpheres();

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

    std::cout << '\n';

    image.Save("img23.png");

    return 0;
}