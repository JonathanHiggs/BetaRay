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

        Camera(Point from, Point at, Vec up, Scalar vfov, Scalar aspectRatio)
        {
            auto theta = glm::radians(vfov);
            auto h = glm::tan(theta / 2.0);

            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            auto w = glm::normalize(from - at);
            auto u = glm::normalize(glm::cross(up, w));
            auto v = glm::normalize(glm::cross(w, u));

            Origin = from;
            Horizontal = viewportWidth * u;
            Vertical = viewportHeight * v;
            LowerLeft = Origin - Horizontal / 2.0 - Vertical / 2.0 - w;
        }

        Ray RayFromViewport(Scalar s, Scalar t) const
        {
            return Ray(Origin, LowerLeft + s * Horizontal + t * Vertical - Origin);
        }
    };

}