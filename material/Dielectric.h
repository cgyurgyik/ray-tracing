#ifndef RAYTRACING_DIELECTRIC_H
#define RAYTRACING_DIELECTRIC_H
#include "Material.h"
#include <cmath>

// Supported dielectric refractive indices.
enum DIELECTRIC_MATERIAL_REFRACTIVE_INDEX {
    AIR,
    GLASS_LOWER,
    GLASS_MID,
    GLASS_HIGH,
    DIAMOND
};

// Returns the estimated refractive index for each dielectric material.
value_type get_refractive_index(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX refractive_index) {
    switch (refractive_index) {
        case DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::AIR : return 1.0;
        case DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_LOWER : return 1.3;
        case DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID : return 1.5;
        case DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_HIGH : return 1.7;
        case DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::DIAMOND : return 2.4;
    }
}

// Represent dielectric material (water, glass, diamonds, etc.)
// When a ray hits it, the ray is split into a reflected ray and
// refracted (or transmitted) ray.
class Dielectric : public Material {
public:
    explicit Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX refractive_index) :
    refractive_index_{get_refractive_index(refractive_index)} {}

    // Simple polynomial approximation for glass reflectivity produced
    // by Christophe Schlick.
    value_type schlick(value_type cosine) const {
        const value_type sqrt_r0 = (1 - refractive_index_) / ( 1 + refractive_index_);
        const value_type r0 = sqrt_r0 * sqrt_r0;
        return r0 + (1 - r0) * std::pow((1-cosine), 5);
    }

    // Represents refraction for a dielectric material.
    bool refract(const UnitVec3& v, const UnitVec3& normal, value_type ni_over_nt, UnitVec3& refracted) const {
       const value_type dt = normal.to_free().dot(v.to_free());
       const value_type discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
       if (discriminant <= 0) return false;
       refracted = UnitVec3(((v.to_free() - (normal * dt)) * ni_over_nt) - normal * std::sqrt(discriminant));
       return true;
    }

    // Scattering with dielectric material. If the ray is in a material with
    // a higher refractive index, Snell's law has no real solution, i.e. refraction
    // is not possible. This means all light is reflected internally inside the
    // solid object, also known as "total internal reflection."
    // Note also, that attenuation is always 1; a dielectric surface absorbs nothing.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color3& attenuation, Ray& scattered) const {
        UnitVec3 outward_normal;
        const UnitVec3 reflected = reflect(ray_in.direction(), record.normal);
        value_type ni_over_nt;
        attenuation = Color3(1.0, 1.0, 1.0);
        UnitVec3 refracted;

        value_type reflect_probability;
        value_type cosine;

        const value_type dot_r_n = ray_in.direction().to_free().dot(record.normal);
        if (dot_r_n > 0.0) {
            outward_normal = UnitVec3(-record.normal);
            ni_over_nt = refractive_index_;
            cosine = refractive_index_ * dot_r_n;
        } else {
            outward_normal = UnitVec3(record.normal);
            ni_over_nt = 1.0 / refractive_index_;
            cosine = -dot_r_n;
        }
        const bool is_refracted = refract(ray_in.direction(), outward_normal, ni_over_nt, refracted);
        reflect_probability = is_refracted ? schlick(cosine) : 1.0;

       if (random_value() < reflect_probability) {
           scattered = Ray(record.point_at_parameter, reflected, ray_in.time());
       } else {
           scattered = Ray(record.point_at_parameter, refracted);
       }
       return true;
    }
private:
    const value_type refractive_index_;
};

#endif //RAYTRACING_DIELECTRIC_H
