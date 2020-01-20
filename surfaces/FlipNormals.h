#ifndef RAYTRACING_FLIPNORMALS_H
#define RAYTRACING_FLIPNORMALS_H
#include "Hittable.h"

// Takes a hittable object and flips its normal.
class FlipNormals : public Hittable {
public:
    FlipNormals(std::shared_ptr<const Hittable> hittable_pointer) : hittable_pointer_{hittable_pointer} {}

    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max, HitRecord& record) const override {
        if (hittable_pointer_->hit(ray, t_min, t_max, record)) {
            record.normal = -record.normal;
            return true;
        }
        return false;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const override {
        return hittable_pointer_->bounding_box(t0, t1, box);
    }

private:
    std::shared_ptr<const Hittable> hittable_pointer_;
};

#endif //RAYTRACING_FLIPNORMALS_H
