#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H
#include "../utility/Vec3.h"
#include "../utility/Ray.h"
#include "AxisAlignedBoundingBox.h"

class Material; // To avoid circularity of dependencies.

// A record to determine necessary attributes for a hit.
struct HitRecord {
    value_type hit_point;
    BoundVec3 point_at_parameter;
    FreeVec3 normal;
    value_type u; // Used for 2-dimensional
    value_type v; // texture maps.
    Material* material_pointer;
};

// Represents an object with a hittable surface.
class Hittable {
public:
    // Given a valid interval [t_min, t_max], the ray is considered a 'hit' if it lies
    // within these intervals. This will always continue looking for the closest hit rather than the
    // first hit.
    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max, HitRecord& record) const = 0;

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const = 0;
};

#endif //RAYTRACING_HITTABLE_H
