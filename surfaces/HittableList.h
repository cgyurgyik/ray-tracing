#ifndef RAYTRACING_HITTABLELIST_H
#define RAYTRACING_HITTABLELIST_H
#include "Hittable.h"

// TODO: Use a vector instead of an array.
class HittableList : public Hittable {
public:
    HittableList(Hittable **list, int list_size) : list_size_{list_size}, list_{list} {}

    bool hit(const Ray &ray, value_type t_min, value_type t_max, HitRecord &record) const {
        HitRecord temp_record;
        bool hit_anything = false;
        value_type closest_hit = t_max;
        for (int i = 0; i < list_size_; ++i) {
            if (list_[i]->hit(ray, t_min, closest_hit, temp_record)) {
                hit_anything = true;
                closest_hit = temp_record.hit_point;
                record = temp_record;
            }
        }
        return hit_anything;
    }

    bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        if (list_size_ <= 0) return false;
        AxisAlignedBoundingBox temp_box;
        const bool first_true = list_[0]->bounding_box(t0, t1, temp_box);
        if (!first_true) return false;
        box = temp_box;
        for (int i = 1; i < list_size_; ++i) {
            if (list_[i]->bounding_box(t0, t1, temp_box)) {
                box = AxisAlignedBoundingBox::surrounding_box(box, temp_box);
            } else { return false; }
        }
        return true;
    }

private:
    Hittable **list_;
    const int list_size_;
};

#endif //RAYTRACING_HITTABLELIST_H
