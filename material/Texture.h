#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "../utility/Vec3.h"

// Represents the colors on a surface procedural.
class Texture {
public:
    virtual Color3 value(value_type u, value_type v, const BoundVec3& p) const = 0;
};

#endif //RAYTRACING_TEXTURE_H
