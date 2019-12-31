#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "Vec3.h"
#include "Hittable.h"

// Represents the behavior of material, or how a ray may react to certain materials.
class Material {
public:
    // Incorporates two main ideas:
    // 1. Produce a scattered ray (or the resulting absorption).
    // 2. If scattered, say how much the ray should be attenuated.
    // Simply put, this tells us how the the ray will interact with the surface.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color3& attenuation, Ray& scattered) const = 0;
};

#endif //RAYTRACING_MATERIAL_H
