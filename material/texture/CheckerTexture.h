#ifndef RAYTRACING_CHECKERTEXTURE_H
#define RAYTRACING_CHECKERTEXTURE_H
#include "Texture.h"

// Represents a checkered (2-color) texture over a material.
// The entire material will hold a checkered pattern.
class CheckerTexture : public Texture {
public:
    CheckerTexture(std::shared_ptr<const Texture> odd, std::shared_ptr<const Texture> even) :
                   odd_{odd}, even_{even} {}

    // Creates a checkered 3-dimensional pattern using the alternating signs of
    // cosine and sine.
    virtual Color3 value(value_type u, value_type v, const BoundVec3& p) const override {
        const value_type sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) return odd_->value(u, v, p);
        return even_->value(u, v, p);
    }
    
private:
    // The texture of the odd checkers.
    std::shared_ptr<const Texture> odd_;
    // The texture of the even checkers.
    std::shared_ptr<const Texture> even_;
};

#endif //RAYTRACING_CHECKERTEXTURE_H
