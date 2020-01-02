#ifndef RAYTRACING_RECTANGLE_XZ_H
#define RAYTRACING_RECTANGLE_XZ_H
#include "Hittable.h"
#include "AxisAlignedBoundingBox.h"

// Axis aligned rectangle in the XZ directions.
// This means the plane is defined by its y value, i.e. y = k.
class Rectangle_XZ : public Hittable {
public:
    Rectangle_XZ(value_type x0, value_type x1, value_type z0, value_type z1, value_type k, Material* material_pointer) :
            x0_{x0}, x1_{x1}, z0_{z0}, z1_{z1}, k_{k}, material_pointer_{material_pointer} {}

    // It is considered a hit if x0_ x < x1_ and z0_ < z < z1_.
    // Recall y = k_. We can calculate t = (k - a_y) / b_y.
    // -> x = a_x + t * b_x, and z = a_z + t * b_z.
    // The normal is then calculated to be (0, 1, 0) (y-axis).
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        const value_type t = (k_ - ray.origin().y()) / ray.direction().y();
        if (t < t0 || t > t1) return false;
        const value_type x = ray.origin().x() + ray.direction().x() * t;
        const value_type z = ray.origin().z() + ray.direction().z() * t;
        const bool outside_x_bounds = x < x0_ || x > x1_;
        const bool outside_z_bounds = z < z0_ || z > z1_;
        if (outside_x_bounds || outside_z_bounds) return false;

        get_rectangle_XZ_uv(x, z, record.u, record.v);
        record.hit_point = t;
        record.point_at_parameter = ray.point_at_parameter(t);
        record.normal = FreeVec3(0, 1, 0);
        record.material_pointer = material_pointer_;
        return true;
    }

    // TODO: Magic number; either explain or fix.
    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(BoundVec3(x0_, k_ - 0.0001, z0_), BoundVec3(x1_, k_ + 0.0001, z1_));
        return true;
    }

private:
    // Used to produce 2-dimensional texture coordinates for a rectangular XZ aligned surface.
    void get_rectangle_XZ_uv(value_type x, value_type z, value_type& u, value_type& v) const {
        u = (x - x0_) / (x1_ - x0_);
        v = (z - z0_) / (z1_ - z0_);
    }

    // x0_, x1_, z0_, z1_ are the four corner points.
    // k_ is the y-coordinate.
    const value_type x0_, x1_, z0_, z1_, k_;
    // The associated material of the rectangular surface.
    Material* material_pointer_;
};

#endif //RAYTRACING_RECTANGLE_XZ_H
