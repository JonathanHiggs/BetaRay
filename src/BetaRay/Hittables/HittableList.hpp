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

#if 1
            // ToDo: Benchmark
            if (std::ranges::empty(hits))
                return std::nullopt;

            return *std::ranges::min_element(hits, compareByDistance);
#else
            auto it = std::rnages::min_element(hits, compare);
            return it != std::end(hits) ? *it : std::nullopt;
#endif
        }

        BoundingBox Bounds(Scalar time0, Scalar time1) const override
        {
#if 1
            BoundingBox box;

            for (auto const & object : Objects)
                box = BoundingBox::Surround(box, object->Bounds(time0, time1));

            return box;
#else
            auto surround = [=](shared_ptr const & a, shared_ptr const & b) {
                return BoundingBox::Surround(a->Bounds(time0, time1), b->Bounds(time0, time1)); }

            return std::accumulate(std::begin(Objects), std::end(Objects), BoundingBox(), surround);
#endif
        }
    };

}