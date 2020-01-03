#ifndef RAYTRACING_HITTABLECOLLECTION_H
#define RAYTRACING_HITTABLECOLLECTION_H
#include "Hittable.h"
#include <vector>

// Encapsulates a collection of hittables.
class HittableCollection : public Hittable {
public:
    HittableCollection() {}

    bool hit(const Ray &ray, value_type t_min, value_type t_max, HitRecord &record) const {
        HitRecord temp_record;
        bool hit_anything = false;
        value_type closest_hit = t_max;
        for (int i = 0; i < hittables_.size(); ++i) {
            if (hittables_[i]->hit(ray, t_min, closest_hit, temp_record)) {
                hit_anything = true;
                closest_hit = temp_record.hit_point;
                record = temp_record;
            }
        }
        return hit_anything;
    }

    bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        const size_t list_size = hittables_.size();
        if (list_size <= 0) return false;
        AxisAlignedBoundingBox temp_box;
        const bool first_true = hittables_[0]->bounding_box(t0, t1, temp_box);
        if (!first_true) return false;
        box = temp_box;
        for (int i = 1; i < list_size; ++i) {
            if (hittables_[i]->bounding_box(t0, t1, temp_box)) {
                box = AxisAlignedBoundingBox::surrounding_box(box, temp_box);
            } else { return false; }
        }
        return true;
    }

    // Stores the pointer to each hittable.
    std::vector<std::shared_ptr<Hittable>> hittables_;
};

#endif //RAYTRACING_HITTABLECOLLECTION_H
