#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H
#include "Material.h"
#include "texture/Texture.h"
#include "../utility/Ray.h"
#include "../utility/util.h"

// Represents Lambertian (diffusion) case.
class Lambertian : public Material {
public:
    explicit Lambertian(std::shared_ptr<const Texture> albedo) : albedo_{albedo} {}

    // There are two circumstances with the Lambertian scatter case:
    // 1. Scatter always and attenuate by its reflectance R.
    // 2. Scatter with no attenuation but absorb the fraction (1 - R) of the rays.
    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Color3& attenuation, Ray& scattered) const override {
        OrthonormalBasis3 uvw;
        uvw.build_from_w(UnitVec3(record.normal));
        const UnitVec3 direction = UnitVec3(uvw.local(random_cosine_direction()));
        const BoundVec3 point_at_parameter = record.point_at_parameter;
        scattered = Ray(point_at_parameter, direction, ray_in.time());
        attenuation = albedo_->value(record.u, record.v, point_at_parameter);
        return true;
    }
private:
    std::shared_ptr<const Texture> albedo_;
};

#endif //RAYTRACING_LAMBERTIAN_H
