#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H
#include "Material.h"
#include "Ray.h"
#include "util.h"

// Represents Lambertian (diffusion) case.
class Lambertian : public Material {
public:
    explicit Lambertian(const Color3& albedo) : albedo_{albedo} {}

    // There are two circumstances with the Lambertian scatter case:
    // 1. Scatter always and attenuate by its reflectance R.
    // 2. Scatter with no attenuation but absorb the fraction (1 - R) of the rays.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color3& attenuation, Ray& scattered) const {
        const BoundVec3 point_at_parameter = record.point_at_parameter;
        const BoundVec3 target = point_at_parameter + record.normal + random_value_in_unit_sphere();
        scattered = Ray(point_at_parameter, UnitVec3(target - point_at_parameter));
        attenuation = albedo_;
        return true;
    }
private:
    Color3 albedo_;
};


#endif //RAYTRACING_LAMBERTIAN_H
