#pragma once

#include <BetaRay/Ray.hpp>


namespace BetaRay::Hittables
{

    class BoundingBox
    {
    public:
        Point Minimum;
        Point Maximum;

        BoundingBox() : Minimum(0.0), Maximum(0.0) { }

        BoundingBox(Point const & minimum, Point const & maximum)
            : Minimum(minimum), Maximum(maximum)
        { }

        static BoundingBox Surround(BoundingBox const & a, BoundingBox const & b)
        {
            auto minimum = Point(
                glm::min(a.Minimum.x, b.Minimum.x),
                glm::min(a.Minimum.y, b.Minimum.y),
                glm::min(a.Minimum.z, b.Minimum.z));

            auto maximum = Point(
                glm::max(a.Maximum.x, b.Maximum.x),
                glm::max(a.Maximum.y, b.Maximum.y),
                glm::max(a.Maximum.z, b.Maximum.z));

            return BoundingBox(minimum, maximum);
        }

        bool Hit(Ray const & ray, Scalar tMin, Scalar tMax) const
        {
            for (auto dim = 0u; dim < 3u; ++dim)
            {
                #if 1
                auto t0 = glm::min(
                    (Minimum[dim] - ray.Origin[dim]) / ray.Direction[dim],
                    (Maximum[dim] - ray.Origin[dim]) / ray.Direction[dim]);
                auto t1 = glm::max(
                    (Minimum[dim] - ray.Origin[dim]) / ray.Direction[dim],
                    (Maximum[dim] - ray.Origin[dim]) / ray.Direction[dim]);
                #else
                auto t0 = (Minimum[dim] - ray.Origin[dim]) / ray.Direction[dim];
                auto t1 = (Maximum[dim] - ray.Origin[dim]) / ray.Direction[dim];

                if (t0 > t1)
                    std::swap(t0, t1);
                #endif

                #if 1
                tMin = glm::max(t0, tMin);
                tMax = glm::min(t1, tMax);
                #else
                tMin = t0 > tMin ? t0 : tMin;
                tMax = t1 < tMax ? t1 : tMax;
                #endif

                if (tMax <= tMin)
                    return false;
            }

            return true;
        }
    };

    using BoundingBoxOpt = std::optional<BoundingBox>;

}