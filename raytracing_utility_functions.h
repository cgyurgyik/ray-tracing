#ifndef RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#define RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#include "Vec3.h"

// Represents a structure to hold information for a hit point.
// If the surface has been hit, is_hit is true. Otherwise, it is false.
// 'hit_point' represents where the object has been hit.
// In the case of a sphere, this can be determined by the quadratic formula.
struct HitPoint {
    bool is_hit;
    value_type hit_point;
};

// Determines whether a ray has hit a sphere with the given
// center and radius. The formula is generated as follows:
// -> (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = R^2  (sphere centered at Cx, Cy, Cz)
// -> Since a vector from center C to a point P = (x, y, z) is (P - C)
// -> Then, dot((p - C), (p - C)) = R^2
// -> = dot((p(t) - C), p(t) - C)) = R^2
// -> = dot((A + t * B - C), A + t * B - C)) = R^2
// -> = t^2 * dot(B, B) + 2t * dot(B, A - C) + dot (A - C, A - C) - R^2 = 0
bool hit_sphere(const FreeVec3& center, value_type radius, const Ray& ray) {
    const BoundVec3 oc = ray.origin() - center;
    const FreeVec3 direction = ray.direction().to_free();
    const value_type a = direction.dot(direction);
    const value_type b = 2.0 * direction.dot(oc) - radius * radius;
    const value_type c = oc.dot(oc);
    const value_type discriminant = (b * b) - (4 * a * c);
    return (discriminant > 0);
}

// Returns the hit point of a ray on a sphere with a given center and radius.
// If it does not hit the sphere, returns -1.0
HitPoint hit_point_on_sphere(const FreeVec3& center, value_type radius, const Ray& ray) {
    const BoundVec3 oc = ray.origin() - center;
    const FreeVec3 direction = ray.direction().to_free();
    const value_type a = direction.dot(direction);
    const value_type b = 2.0 * direction.dot(oc) - radius * radius;
    const value_type c = oc.dot(oc);
    const value_type discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) return HitPoint{.is_hit=false};
    return HitPoint{.is_hit=true, .hit_point=(-b - std::sqrt(discriminant)) / (2.0 * a)};
}

Color3 color(const Ray& ray) {
    const HitPoint hit_point = hit_point_on_sphere(FreeVec3(0, 0, -1), 0.5, ray);
    if (hit_point.is_hit) {
        BoundVec3 normal = (ray.point_at_parameter(hit_point.hit_point) - FreeVec3(0, 0, -1));
        return Color3(FreeVec3(normal.x()+1, normal.y()+1, normal.z()+1) * 0.5);
    }
    const UnitVec3 unit_direction = ray.direction();
    const value_type t = 0.5 * (unit_direction.y() + 1.0);
    return Color3(FreeVec3(1.0, 1.0, 1.0) * (1.0 - t) + (FreeVec3(0.5, 0.7, 1.0) * t));
}



#endif //RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
