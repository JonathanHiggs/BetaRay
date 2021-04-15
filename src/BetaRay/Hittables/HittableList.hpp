#pragma once

#include <BetaRay/Hittables/IHittable.hpp>


namespace BetaRay::Hittables
{

    class HittableList : public IHittable
    {
    public:
        std::vector<shared_ptr> Objects{};

        HitResultOpt Hit(Ray const & ray, Scalar tMin, Scalar tMax) const override
        {
            auto testHit = [&](auto const & x) { return x->Hit(ray, tMin, tMax); };
            auto filterMiss = [](auto const & x) { return x.has_value(); };

            auto compareByDistance =
                [](auto const & a, auto const & b) { return a.value().Intersect < b.value().Intersect; };

            auto hits = Objects | std::views::transform(testHit) | std::views::filter(filterMiss);

            // ToDo: Benchmark
            if (std::ranges::empty(hits))
                return std::nullopt;

            return *std::ranges::min_element(hits, compareByDistance);
        }

        BoundingBox Bounds(Scalar time0, Scalar time1) const override
        {
            auto surround = [=](BoundingBox const & box, shared_ptr const & hittable) {
                return BoundingBox::Surround(box, hittable->Bounds(time0, time1)); };

            return std::accumulate(std::begin(Objects), std::end(Objects), BoundingBox(), surround);
        }
    };

}