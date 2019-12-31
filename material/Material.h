#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "../Vec3.h"
#include "../Hittable.h"

// Represents the behavior of material, or how a ray may react to certain materials.
class Material {
public:
    // Incorporates two main ideas:
    // 1. Produce a scattered ray (or the resulting absorption).
    // 2. If scattered, say how much the ray should be attenuated.
    // Simply put, this tells us how the the ray will interact with the surface.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color3& attenuation, Ray& scattered) const = 0;

    // For smooth metals, rays will not be randomly scattered.
    // Instead, the metal is treated as a mirror, and we can
    // calculate the reflection as:
    // v - 2B, where v is the ray's direction, and B = dot(v, N).
    // In this case, N is the normal to the metal surface.
    // Since v points inward, 2B will be negated.
    inline UnitVec3 reflect(const UnitVec3& v, const FreeVec3 normal) const {
        return   UnitVec3(v.to_free() - (normal *  2 * normal.dot(v.to_free())));
    }
};

#endif //RAYTRACING_MATERIAL_H
