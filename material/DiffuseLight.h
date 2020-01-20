#ifndef RAYTRACING_DIFFUSELIGHT_H
#define RAYTRACING_DIFFUSELIGHT_H
#include "Material.h"
#include "texture/Texture.h"

// A light emitting material.
class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<const Texture> emit) : emit_{emit} {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record,
                         Color3& attenuation, Ray& scattered) const override {
        return false;
    }

    virtual Color3 emitted(value_type u, value_type v, const BoundVec3& p) const override {
        return emit_->value(u, v, p);
    }
private:
    std::shared_ptr<const Texture> emit_;
};

#endif //RAYTRACING_DIFFUSELIGHT_H
