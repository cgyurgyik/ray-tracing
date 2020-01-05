#ifndef RAYTRACING_TRIANGLE_XY_H
#define RAYTRACING_TRIANGLE_XY_H

#include <memory>
#include "../material/Material.h"
#include "Hittable.h"

// Axis aligned triangle in the XY directions.
// This means the plane is defined by its z value, i.e. z = k.
class Triangle_XY : public Hittable {
public:
    Triangle_XY() {}

    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        assert(false);
        return false;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        assert(false);
        return false;
    }

private:
    // The associated material of the triangular surface.
    std::shared_ptr<const Material> material_;
};

#endif //RAYTRACING_TRIANGLE_XY_H
