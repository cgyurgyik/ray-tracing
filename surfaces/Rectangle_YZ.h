#ifndef RAYTRACING_RECTANGLE_YZ_H
#define RAYTRACING_RECTANGLE_YZ_H
#include "Hittable.h"
#include "AxisAlignedBoundingBox.h"

// Axis aligned rectangle in the YZ directions.
// This means the plane is defined by its x value, i.e. x = k.
class Rectangle_YZ : public Hittable {
public:
    Rectangle_YZ(value_type y0, value_type y1, value_type z0, value_type z1, value_type k, Material* material_pointer) :
            y0_{y0}, y1_{y1}, z0_{z0}, z1_{z1}, k_{k}, material_pointer_{material_pointer} {}

    // It is considered a hit if y0_ y < y1_ and z0_ < z < z1_.
    // Recall x = k_. We can calculate t = (k - a_x) / b_x.
    // -> y = a_y + t * b_y, and z = a_z + t * b_z.
    // The normal is then calculated to be (1, 0, 0) (x-axis).
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        const value_type t = (k_ - ray.origin().x()) / ray.direction().x();
        if (t < t0 || t > t1) return false;
        const value_type y = ray.origin().y() + ray.direction().y() * t;
        const value_type z = ray.origin().z() + ray.direction().z() * t;
        const bool outside_x_bounds = y < y0_ || y > y1_;
        const bool outside_z_bounds = z < z0_ || z > z1_;
        if (outside_x_bounds || outside_z_bounds) return false;

        get_rectangle_YZ_uv(y, z, record.u, record.v);
        record.hit_point = t;
        record.point_at_parameter = ray.point_at_parameter(t);
        record.normal = FreeVec3(1, 0, 0);
        record.material_pointer = material_pointer_;
        return true;
    }

    // TODO: Magic number; either explain or fix.
    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(BoundVec3(k_ - 0.0001, y0_, z0_), BoundVec3(k_ + 0.0001, y1_, z1_));
        return true;
    }

private:
    // Used to produce 2-dimensional texture coordinates for a rectangular XZ aligned surface.
    void get_rectangle_YZ_uv(value_type y, value_type z, value_type& u, value_type& v) const {
        u = (y - y0_) / (y1_ - y0_);
        v = (z - z0_) / (z1_ - z0_);
    }

    // x0_, x1_, z0_, z1_ are the four corner points.
    // k_ is the y-coordinate.
    const value_type y0_, y1_, z0_, z1_, k_;
    // The associated material of the rectangular surface.
    Material* material_pointer_;
};

#endif //RAYTRACING_RECTANGLE_YZ_H
