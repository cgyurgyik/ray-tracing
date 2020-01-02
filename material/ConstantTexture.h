#ifndef RAYTRACING_CONSTANTTEXTURE_H
#define RAYTRACING_CONSTANTTEXTURE_H
#include "Texture.h"

// Represents a constant texture over a material.
// The entire material will hold a single color vector.
class ConstantTexture : public Texture {
public:
    ConstantTexture(const Color3& color) : color_{color} {}

    virtual Color3 value(value_type u, value_type v, const BoundVec3& p) const {
        return color_;
    }
private:
    // The color of the constant texture.
    const Color3 color_;
};

#endif //RAYTRACING_CONSTANTTEXTURE_H
