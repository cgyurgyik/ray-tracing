#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H

#include "Material.h"
#include "../utility/Vec3.h"
#include "../utility/util.h"

// Represents a metal surface. The general trend is, the bigger the surface,
// the fuzzier the reflection will be. This class allows for a fuzz parameter
// with bounds [0, 1].
class Metal: public Material {
public:
    explicit Metal(const Color3& albedo, value_type fuzz) : albedo_{albedo}, fuzz_{fuzz < 1 ? fuzz : 1} {}

    // In this case, the scatter is a simple reflection.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Color3& attenuation, Ray& scattered) const override {
        const FreeVec3 reflected = reflect(ray_in.direction(), record.normal).to_free();
        const FreeVec3 fuzzed = FreeVec3(random_value_in_unit_sphere() * fuzz_);
        scattered = Ray(record.point_at_parameter, UnitVec3(reflected + fuzzed), ray_in.time());
        attenuation = albedo_;
        return scattered.direction().to_free().dot(record.normal) > 0;
    }
private:
    Color3 albedo_;
    value_type fuzz_;
};
#endif //RAYTRACING_METAL_H
