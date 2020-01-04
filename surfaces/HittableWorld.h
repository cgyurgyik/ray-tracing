#ifndef RAYTRACING_HITTABLEWORLD_H
#define RAYTRACING_HITTABLEWORLD_H
#include "Hittable.h"
#include <vector>

// Encapsulates a collection of hittables.
class HittableWorld : public Hittable {
public:
    HittableWorld() {}

    // Reserves space for 'size' number of hittables.
    HittableWorld(int size) {hittables_.reserve(size); }

    // Adds a hittable surface to the current world.
    // The size also increments.
    void add(std::shared_ptr<Hittable> hittable) {
        hittables_.push_back(hittable);
    }

    // Returns the number of hittables in the current world.
    size_t size() {
        return hittables_.size();
    }

    // Clears the hittable world, removing all hittable surfaces.
    // The size of the world will be zero.
    void clear() {
        hittables_.clear();
    }

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

private:
    // Stores the pointer to each hittable.
    std::vector<std::shared_ptr<Hittable>> hittables_;
};

#endif //RAYTRACING_HITTABLEWORLD_H
