#ifndef RAYTRACING_HITTABLELIST_H
#define RAYTRACING_HITTABLELIST_H
#include "Hittable.h"

class HittableList : public Hittable {
public:
    HittableList(Hittable **list, int list_size) : list_size_{list_size}, list_{list} {}
    virtual bool hit(const Ray& ray, value_type minimum, value_type maximum, HitRecord& record) const;
private:
    Hittable **list_;
    const int list_size_;
};

bool HittableList::hit(const Ray &ray, value_type minimum, value_type maximum, HitRecord &record) const {
    HitRecord temp_record{.point_at_parameter=Vec3(0.0,0.0,0.0), .normal=Vec3(0.0,0.0,0.0)};
    bool hit_anything = false;
    value_type closest_hit = maximum;
    for (int i = 0; i < list_size_; ++i) {
        if (list_[i]->hit(ray, minimum, closest_hit, temp_record)) {
            hit_anything = true;
            closest_hit = temp_record.hit_point;
            record = temp_record;
        }
    }
    return hit_anything;
}

#endif //RAYTRACING_HITTABLELIST_H
