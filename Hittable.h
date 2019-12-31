#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H
#include "Vec3.h"
#include "Ray.h"

class Material; // To avoid circularity of dependencies.

// A record to determine necessary attributes for a hit.
struct HitRecord {
    value_type hit_point;
    BoundVec3 point_at_parameter = BoundVec3(0.0,0.0,0.0);
    FreeVec3 normal = FreeVec3(0.0,0.0,0.0);
    Material* material_pointer;
};

// Represents an object with a hittable surface.
class Hittable {
public:
    virtual bool hit(const Ray& ray, value_type minimum, value_type maximum, HitRecord& record) const = 0;
};

#endif //RAYTRACING_HITTABLE_H
