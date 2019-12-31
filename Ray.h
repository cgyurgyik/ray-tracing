#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include "Vec3.h"


// Represents a computation of what color is seen along a ray.
struct Ray {
    Ray() : origin_{BoundVec3(0.0,0.0,0.0)}, direction_{UnitVec3(0.0,0.0,0.0)} {}
    constexpr Ray(const BoundVec3& origin, const UnitVec3& direction)
            : origin_{origin}, direction_{direction} {}

    // Represents the function p(t) = origin + t * direction,
    // where p is a 3-dimensional position, and t is a scalar.
    constexpr BoundVec3 point_at_parameter(const value_type t) const {
        return this->origin_ + (this->direction_ * t);
    }

    inline constexpr BoundVec3 origin() const { return this->origin_; }
    inline UnitVec3 direction() const { return this->direction_; }

private:
    BoundVec3 origin_;
    UnitVec3 direction_;
};
#endif //RAYTRACING_RAY_H