#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H
#include "Vec3.h"

// A record to determine necessary attributes for a hit.
struct HitRecord {
    bool is_hit;
    value_type hit_point;
    Vec3 point_at_parameter = Vec3(0.0,0.0,0.0);
    Vec3 normal = Vec3(0.0,0.0,0.0);
};

// Represents an object with a hittable surface.
class Hittable {
public:
    virtual bool hit(const Ray& ray, value_type minimum, value_type maximum, HitRecord& record) const = 0;
};

#endif //RAYTRACING_HITTABLE_H
