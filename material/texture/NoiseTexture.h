#ifndef RAYTRACING_NOISETEXTURE_H
#define RAYTRACING_NOISETEXTURE_H
#include "Texture.h"
#include "Perlin.h"

// Represents a constant texture over a material.
// The entire material will hold a single color vector.
// The turbulence depth determines how many summed frequencies,
// or calls to noise occurs.
// TODO: Finish documentation.
class NoiseTexture : public Texture {
public:
    NoiseTexture(int turbulence_depth, int scale) : turbulence_depth_{turbulence_depth}, scale_{scale} {}

    // Uses the sine function to allow for basic color proportionality, and then adjusts the value accordingly
    // using the turbulence.
    virtual Color3 value(value_type u, value_type v, const BoundVec3& p) const {
        return Color3(1.0, 1.0, 1.0) * 0.5 * (1 + sin(scale_ * p.z() + 10 * noise_.turbulence(p, turbulence_depth_)));
    }
private:
    // The Perlin random permutation generator.
    Perlin noise_;
    const int turbulence_depth_;
    const int scale_;
};

#endif //RAYTRACING_NOISETEXTURE_H
