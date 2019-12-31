#ifndef RAYTRACING_HITTABLELIST_H
#define RAYTRACING_HITTABLELIST_H
#include "Hittable.h"

class HittableList : public Hittable {
public:
    HittableList(Hittable **list, int list_size) : list_size_{list_size}, list_{list} {}

    // Given a valid interval [t_min, t_max], the ray is considered a 'hit' if it lies
    // within these intervals. This will always continue looking for the closest hit rather than the
    // first hit.
    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max, HitRecord& record) const;
private:
    Hittable **list_;
    const int list_size_;
};

bool HittableList::hit(const Ray &ray, value_type t_min, value_type t_max, HitRecord &record) const {
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

#endif //RAYTRACING_HITTABLELIST_H
