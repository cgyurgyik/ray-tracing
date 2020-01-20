#ifndef RAYTRACING_RECTANGLE_XY_H
#define RAYTRACING_RECTANGLE_XY_H
#include "Hittable.h"
#include "AxisAlignedBoundingBox.h"

// Axis aligned rectangle in the XY directions.
// This means the plane is defined by its z value, i.e. z = k.
class Rectangle_XY : public Hittable {
public:
    Rectangle_XY(value_type x0, value_type x1, value_type y0, value_type y1, value_type k,
                 std::shared_ptr<const Material> material) :
    x0_{x0}, x1_{x1}, y0_{y0}, y1_{y1}, k_{k}, material_{material} {}

    // It is considered a hit if x0_ x < x1_ and y0_ < y < y1_.
    // Recall z = k_. We can calculate t = (k - a_z) / b_z.
    // -> x = a_x + t * b_x, and y = a_y + t * b_y.
    // The normal is then calculated to be (0, 0, 1) (z-axis).
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const override {
        const value_type t = (k_ - ray.origin().z()) / ray.direction().z();
        if (t < t0 || t > t1) return false;
        const value_type x = ray.origin().x() + ray.direction().x() * t;
        const value_type y = ray.origin().y() + ray.direction().y() * t;
        const bool outside_x_bounds = x < x0_ || x > x1_;
        const bool outside_y_bounds = y < y0_ || y > y1_;
        if (outside_x_bounds || outside_y_bounds) return false;

        record.u = (x - x0_) / (x1_ - x0_);
        record.v = (y - y0_) / (y1_ - y0_);
        record.hit_point = t;
        record.point_at_parameter = ray.point_at_parameter(t);
        record.normal = FreeVec3(0, 0, 1);
        record.material = material_;
        return true;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const override {
        box = AxisAlignedBoundingBox(BoundVec3(x0_, y0_, k_ - 0.0001), BoundVec3(x1_, y1_, k_ + 0.0001));
        return true;
    }

private:
    // x0_, x1_, y0_, y1_ are the four corner points.
    // k_ is the z-coordinate.
    const value_type x0_, x1_, y0_, y1_, k_;
    // The associated material of the rectangular surface.
    std::shared_ptr<const Material> material_;
};

#endif //RAYTRACING_RECTANGLE_XY_H
