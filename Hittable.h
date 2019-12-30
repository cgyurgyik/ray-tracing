#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H

#include "Vec3.h"
#include "HittableList.h"
#include "raytracing_utility_functions.h"

// A record to determine necessary attributes for a hit.
struct hit_record {
    bool is_hit;
    value_type hit_point;
    Vec3 point_at_parameter;
    Vec3 normal;
};

// Represents an object with a hittable surface.
class Hittable {
public:
    virtual bool hit(const Ray& ray, value_type minimum, value_type maximum, hit_record& record) const = 0;
};

#endif //RAYTRACING_HITTABLE_H
