#ifndef RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#define RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#include "Vec3.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Hittable.h"
#include <limits>
#include <functional>
#include <random>

FreeVec3 color(const Ray& ray, Hittable *world) {
    HitRecord record;
    if (world->hit(ray, 0.0, std::numeric_limits<value_type>::max(), record)) {
        return FreeVec3(record.normal.x()+1, record.normal.y()+1, record.normal.z()+1) * 0.5;
    }
    const UnitVec3 unit_direction = ray.direction();
    const value_type t = 0.5 * (unit_direction.y() + 1.0);
    return FreeVec3(1.0, 1.0, 1.0) * (1.0 - t) + (FreeVec3(0.5, 0.7, 1.0) * t);
}

inline value_type random_value() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<value_type()> rand_generator =
            std::bind(distribution, generator);
    return rand_generator();
}



#endif //RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
