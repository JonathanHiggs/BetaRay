#pragma once

#include <BetaRay/Hittables/HittableList.hpp>


namespace BetaRay::Hittables
{

    class BvhNode : public IHittable
    {
    public:
        shared_ptr Left;
        shared_ptr Right;
        BoundingBox HitBox;

        BvhNode(HittableList const & list, Scalar time0, Scalar time1)
            : BvhNode(list.Objects, 0, list.Objects.size(), time0, time1)
        { }

        BvhNode(
            std::vector<shared_ptr> const & sourceObjects,
            size_t start, size_t end,
            Scalar time0, Scalar time1)
        {
            auto objects = sourceObjects;

            // ToDo: hoist these out
            static std::uniform_int_distribution<size_t> dist(0, 2);
            static std::mt19937 gen;

            // ToDo: better way than guessing an axis?
            int axis = dist(gen);

            auto comparator = [=](shared_ptr const & a, shared_ptr const & b) {
                auto aBox = a->Bounds(time0, time1);
                auto bBox = b->Bounds(time0, time1);
                return aBox.Minimum[axis] < bBox.Minimum[axis]; };

            auto span = end - start;
            assert(span != 1);

            if (span == 2)
            {
                Left  = objects[start];
                Right = objects[start + 1];
            }
            else
            {
                std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

                auto mid = start + span / 2;
                if (span == 3)  Left = objects[start];
                else            Left = std::make_shared<BvhNode>(objects, start, mid, time0, time1);

                Right = std::make_shared<BvhNode>(objects, mid, end, time0, time1);
            }

            auto leftBox = Left->Bounds(time0, time1);
            auto rightBox = Right->Bounds(time0, time1);

            HitBox = BoundingBox::Surround(leftBox, rightBox);
        }

        HitResultOpt Hit(Ray const & ray, Scalar tMin, Scalar tMax) const override
        {
            if (!HitBox.Hit(ray, tMin, tMax))
                return std::nullopt;

            auto leftResult = Left->Hit(ray, tMin, tMax);
            tMax = leftResult.has_value() ? leftResult.value().Intersect : tMax;
            auto rightResult = Right->Hit(ray, tMin, tMax);

            return rightResult.has_value() ? rightResult : leftResult;
        }

        BoundingBox Bounds(Scalar time0, Scalar time1) const override { return HitBox; }
    };

}