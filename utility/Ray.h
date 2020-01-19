#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include "Vec3.h"


// Represents a computation of what color is seen along a ray.
// If no time is given, it is defaulted to 0 seconds.
struct Ray {
    Ray() : origin_{BoundVec3()}, direction_{UnitVec3()}, time_{0.0} {}
    constexpr Ray(const BoundVec3& origin, const UnitVec3& direction, value_type time = 0.0)
            : origin_{origin}, direction_{direction}, time_{time} {}

    // Represents the function p(t) = origin + t * direction,
    // where p is a 3-dimensional position, and t is a scalar.
    constexpr BoundVec3 point_at_parameter(const value_type t) const {
        return this->origin_ + (this->direction_ * t);
    }

    inline constexpr BoundVec3 origin() const { return this->origin_; }
    inline constexpr UnitVec3 direction() const { return this->direction_; }
    inline value_type time() const { return this->time_; }

private:
    // The origin of the ray.
    BoundVec3 origin_;
    // The normalized direction of the ray.
    UnitVec3 direction_;
    // The time of the ray. If none is provided, it defaults to 0 seconds.
    value_type time_;
};

#endif //RAYTRACING_RAY_H