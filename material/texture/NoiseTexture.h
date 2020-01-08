#ifndef RAYTRACING_NOISETEXTURE_H
#define RAYTRACING_NOISETEXTURE_H
#include "Texture.h"
#include "Perlin.h"

// Represents a constant noise texture over a material.
// After turbulence and scaling, the resulting
// texture will produce a marble-like visual.
class NoiseTexture : public Texture {
public:
    NoiseTexture(int turbulence_depth, int scale, Perlin perlin_noise) :
    turbulence_depth_{turbulence_depth}, scale_{scale}, perlin_noise_{perlin_noise} {}

    // Uses the sine function to allow for basic color proportionality, and then adjusts the value accordingly
    // using the turbulence.
    virtual Color3 value(value_type u, value_type v, const BoundVec3& p) const {
        return Color3(1.0, 1.0, 1.0) * 0.5 * (1 + sin(scale_ * p.z() + 10 * perlin_noise_.turbulence(p, turbulence_depth_)));
    }
private:
    // The Perlin random permutation generator.
    Perlin perlin_noise_;
    // The turbulence depth determines how many summed frequencies,
    // or calls to noise occurs.
    const int turbulence_depth_;
    // The amount to scale the z-coordinate when producing Perlin noise.
    const int scale_;
};

#endif //RAYTRACING_NOISETEXTURE_H
