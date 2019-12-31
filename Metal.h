#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H

#include "Material.h"
#include "Vec3.h"
#include "util.h"

// Represents a metal surface. The general trend is, the bigger the surface,
// the fuzzier the reflection will be. This class allows for a fuzz parameter
// with bounds [0, 1].
class Metal: public Material {
public:
    explicit Metal(const Color3& albedo, value_type fuzz) : albedo_{albedo}, fuzz_{fuzz < 1 ? fuzz : 1} {}

    // For smooth metals, rays will not be randomly scattered.
    // Instead, the metal is treated as a mirror, and we can
    // calculate the reflection as:
    // v - 2B, where v is the ray's direction, and B = dot(v, N).
    // In this case, N is the normal to the metal surface.
    // Since v points inward, 2B will be negated.
    inline UnitVec3 reflect(const UnitVec3& v, const FreeVec3 normal) const {
        return   UnitVec3(v.to_free() - (normal *  2 * normal.dot(v.to_free())));
    }

    // In this case, the scatter is a simple reflection.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color3& attenuation, Ray& scattered) const {
        const UnitVec3 reflected = reflect(ray_in.direction(), record.normal);
        scattered = Ray(record.point_at_parameter, reflected);
        attenuation = albedo_;
        return scattered.direction().to_free().dot(record.normal) > 0;
    }
private:
    Color3 albedo_;
    value_type fuzz_;
};
#endif //RAYTRACING_METAL_H
