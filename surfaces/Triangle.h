#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include <memory>
#include "../material/Material.h"
#include "Hittable.h"
#include "AxisAlignedBoundingBox.h"

// Encapsulates a single sided triangle surface.
class Triangle : public Hittable {
public:
    Triangle(const BoundVec3& a, const BoundVec3& b, const BoundVec3& c, std::shared_ptr<const Material> material) : a_{a}, b_{b}, c_{c},
    material_{material} {}

    // A hit inside a triangle is determined by the cross product of its vertices.
    // This is translated mathematically to:
    // (b - a) x (p - a) dot normal > 0
    // (c - b) x (p - b) dot normal > 0
    // (a - c) x (p - c) dot normal > 0
    // If these all return true, it is a hit within the triangle. This is referred
    // to as the "inside-outside" technique, and can be used for any convex polygon.
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        // Produce normal using the cross product.
        const FreeVec3 normal = (b_ - a_).cross((c_ - a_));

        // Ray: p = origin + t * direction.
        // Plane: (p - a).dot(normal) = 0.
        // Substitute p, and we get: (origin + t * direction - a).dot(normal) = 0.
        // Solving for t,
        // t = (a - origin).dot(normal) / (direction.dot(normal)).
        const value_type t = ((a_ - ray.origin()).dot(normal)) / (ray.direction().to_free().dot(normal));
        if (t < t0 || t > t1) return false;

        // Need to determine if the plane hit a point inside the triangle.
        const BoundVec3 p = ray.point_at_parameter(t);
        FreeVec3 C; // Vector perpendicular to triangle's plane.
        value_type u;
        value_type v;

        const FreeVec3 side_1 = b_ - a_;
        const FreeVec3 vp0 = p - a_;
        C = side_1.cross(vp0);
        if (normal.dot(C) < 0) return false;

        const FreeVec3 side_2 = c_ - b_;
        const FreeVec3 vp1 = p - b_;
        C = side_2.cross(vp1);
        if (normal.dot(C) < 0) return false;

        const value_type area_division = 2 / normal.length();
        u = (C.length() / 2) * area_division;

        const FreeVec3 side_3 = a_ - c_;
        const FreeVec3 vp2 = p - c_;
        C = side_3.cross(vp2);
        if (normal.dot(C) < 0) return false;
        v = (C.length() / 2) * area_division;

        record.u = u;
        record.v = v;
        record.hit_point = t;
        record.point_at_parameter = p;
        record.normal = normal;
        record.material = material_;
        return true;
    }

    // Determined by finding minimum & maximum x-, y- and z-coordinates from the three coordinates making up the triangle.
    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        const value_type max_x = get_max(a_.x(), get_max(b_.x(), c_.x()));
        const value_type max_y = get_max(a_.y(), get_max(b_.y(), c_.y()));
        const value_type max_z = get_max(a_.z(), get_max(b_.z(), c_.z()));

        const value_type min_x = get_min(a_.x(), get_min(b_.x(), c_.x()));
        const value_type min_y = get_min(a_.y(), get_min(b_.y(), c_.y()));
        const value_type min_z = get_min(a_.z(), get_min(b_.z(), c_.z()));

        box = AxisAlignedBoundingBox(BoundVec3(min_x, min_y, min_z), BoundVec3(max_x, max_y, max_z));
        return false;
    }

private:
    // The associated material of the triangular surface.
    std::shared_ptr<const Material> material_;
    BoundVec3 a_;
    BoundVec3 b_;
    BoundVec3 c_;
};

#endif //RAYTRACING_TRIANGLE_H
