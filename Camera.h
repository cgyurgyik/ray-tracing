#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "Vec3.h"

// Encapsulates a simple axis-aligned camera.
class Camera {
public:
    Camera(const BoundVec3& origin, const BoundVec3& lower_left_corner,
            const FreeVec3& horizontal, const FreeVec3& vertical) :
            origin_{origin}, lower_left_corner_{lower_left_corner},
            horizontal_{horizontal}, vertical_{vertical} {};
    Ray getRay(value_type u, value_type v) {
        return Ray(origin_, UnitVec3(lower_left_corner_ + (horizontal_ * u) + (vertical_ * v) - origin_));
    }
private:
    const BoundVec3 origin_;
    const BoundVec3 lower_left_corner_;
    const FreeVec3 horizontal_;
    const FreeVec3 vertical_;
};
#endif //RAYTRACING_CAMERA_H
