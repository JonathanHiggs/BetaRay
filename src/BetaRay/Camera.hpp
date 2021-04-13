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

        Camera(
            Point from, Point at, Vec up,
            Scalar vfov, Scalar aspectRatio,
            Scalar aperture, Scalar focalDistance)
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
        }

        Ray RayFromViewport(Scalar s, Scalar t) const
        {
            auto rand = glm::diskRand(LensRadius);
            auto offset = u * rand.x + v * rand.y;

            return Ray(
                Origin + offset,
                LowerLeft + s * Horizontal + t * Vertical - Origin - offset);
        }
    };

}