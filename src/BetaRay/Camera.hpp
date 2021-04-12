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

        Camera(Scalar aspectRatio)
        {
            auto viewportHeight = 2.0;
            auto viewportWidth = aspectRatio * viewportHeight;
            auto focalLength = 1.0;

            Origin = Point(0.0);
            Horizontal = Vec(viewportWidth, 0.0, 0.0);
            Vertical = Vec(0.0, viewportHeight, 0.0);
            LowerLeft = Origin - Horizontal / 2.0 - Vertical / 2.0 - Vec(0.0, 0.0, focalLength);
        }

        Ray RayFromViewport(Scalar u, Scalar v) const
        {
            return Ray(Origin, LowerLeft + u * Horizontal + v * Vertical - Origin);
        }
    };

}