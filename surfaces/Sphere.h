#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H
#include "../utility/Vec3.h"
#include "Hittable.h"

// Represents a 3-dimensional sphere. Each sphere has a center and a radius.
class Sphere : public Hittable {
public:
    Sphere(const BoundVec3& center, value_type radius, std::shared_ptr<Material> material) : center_{center},
    radius_{radius}, material_{material} {}

    // Determines whether a ray has hit a sphere in the boundaries (minimum, maximum)
    // with the given center and radius. The formula is generated as follows:
    // -> (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = R^2  (sphere centered at Cx, Cy, Cz)
    // -> Since a vector from center C to a point P = (x, y, z) is (P - C)
    // -> Then, dot((p - C), (p - C)) = R^2
    // -> = dot((p(t) - C), p(t) - C)) = R^2
    // -> = dot((A + t * B - C), A + t * B - C)) = R^2
    // -> = t^2 * dot(B, B) + 2t * dot(B, A - C) + dot (A - C, A - C) - R^2 = 0
    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max,
                     HitRecord& record) const override {
        const BoundVec3 oc = ray.origin() - center_;
        const FreeVec3 direction = ray.direction().to_free();
        const value_type a = direction.dot(direction);
        const value_type b = direction.dot(oc);
        const value_type c = oc.dot(oc) - (radius_ * radius_);
        const value_type discriminant = (b * b) - (a * c);
        if (discriminant <= 0) return false;
        const value_type hit_point_one = (-b - std::sqrt(discriminant)) / a;
        if (hit_point_one > t_min && hit_point_one < t_max) {
            record.hit_point = hit_point_one;
            record.point_at_parameter = ray.point_at_parameter(hit_point_one);
            record.normal = (FreeVec3(record.point_at_parameter) - center_) / radius_;
            record.material = material_;
            get_sphere_uv(FreeVec3(record.point_at_parameter - center_) / radius_, record.u, record.v);
            return true;
        }
        const value_type hit_point_two = (-b + std::sqrt(discriminant)) / a;
        if (hit_point_two > t_min && hit_point_two < t_max) {
            record.hit_point = hit_point_two;
            record.point_at_parameter = ray.point_at_parameter(hit_point_two);
            record.normal = (FreeVec3(record.point_at_parameter) - center_) / radius_;
            record.material = material_;
            get_sphere_uv(FreeVec3(record.point_at_parameter - center_) / radius_, record.u, record.v);
            return true;
        }
        return false;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const override {
        const FreeVec3 radius_vector(radius_, radius_, radius_);
        box = AxisAlignedBoundingBox(BoundVec3(center_ - radius_vector), BoundVec3(center_ + radius_vector));
        return true;
    }
private:
    // Used to produce 2-dimensional texture coordinates for a spherical surface.
    void get_sphere_uv(const FreeVec3& p, value_type& u, value_type& v) const {
        const value_type phi = atan2(p.z(), p.x());
        const value_type theta = asin(p.y());
        u = 1 - (phi + M_PI) / (2 * M_PI);
        v = (theta + M_PI/2) / M_PI;
    }

    // The center of the sphere.
    const FreeVec3 center_;
    // The radius of the sphere.
    const value_type radius_;
    // The material surface of the sphere.
    std::shared_ptr<Material> material_;
};
#endif //RAYTRACING_SPHERE_H
