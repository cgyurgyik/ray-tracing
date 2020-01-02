#ifndef RAYTRACING_TRANSLATE_H
#define RAYTRACING_TRANSLATE_H
#include "Hittable.h"
#include "HittableList.h"
#include "FlipNormals.h"

// Represents a translation on a hittable object.
class Translate : public Hittable {
public:
    Translate(Hittable* hittable_pointer, const FreeVec3& offset) :
    hittable_pointer_{hittable_pointer}, offset_{offset} {}

    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max, HitRecord& record) const {
        const Ray moved_ray(ray.origin() - offset_, ray.direction(), ray.time());
        if (hittable_pointer_->hit(moved_ray, t_min, t_max, record)) {
            record.point_at_parameter += offset_;
            return true;
        }
        return false;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        if (hittable_pointer_->bounding_box(t0, t1, box)) {
            box = AxisAlignedBoundingBox(box.min() + offset_, box.max() + offset_);
            return true;
        }
        return false;
    }

private:
    Hittable* hittable_pointer_;
    FreeVec3 offset_;
};
#endif //RAYTRACING_TRANSLATE_H
