#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay
{

    class Camera
    {
    public:
        Point Origin;
        Point LowerLeft;
        Vec Horizontal;
        Vec Vertical;
        Vec u, v, w;
        Scalar AspectRatio;
        Scalar LensRadius;
        Scalar FocalDistance;
        Scalar Time0, Time1;

        Camera(
            Point from, Point at, Vec up,
            Scalar vfov, Scalar aspectRatio,
            Scalar aperture, Scalar focalDistance,
            Scalar time0, Scalar time1)
        {
            auto theta = glm::radians(vfov);
            auto h = glm::tan(theta / 2.0);

            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            w = glm::normalize(from - at);
            u = glm::normalize(glm::cross(up, w));
            v = glm::normalize(glm::cross(w, u));

            Origin = from;
            Horizontal = focalDistance * viewportWidth * u;
            Vertical = focalDistance * viewportHeight * v;
            LowerLeft = Origin - Horizontal / 2.0 - Vertical / 2.0 - focalDistance * w;

            AspectRatio = aspectRatio;
            LensRadius = aperture / 2.0;
            FocalDistance = focalDistance;

            Time0 = time0;
            Time1 = time1;
        }

        Ray RayFromViewport(Scalar s, Scalar t) const
        {
            // ToDo: Hoist out
            static std::uniform_real_distribution<Scalar> dist(0.0, 1.0);
            static std::mt19937 gen;

            auto rand = glm::diskRand(LensRadius);
            auto offset = u * rand.x + v * rand.y;
            auto time = dist(gen);

            return Ray(
                Origin + offset,
                LowerLeft + s * Horizontal + t * Vertical - Origin - offset,
                time);
        }
    };

}