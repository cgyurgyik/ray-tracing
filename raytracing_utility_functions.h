#ifndef RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#define RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#include "Vec3.h"
#include "HittableList.h"
#include "Sphere.h"

Color3 color(const Ray& ray, Hittable *world) {
    hit_record record;
    if (world->hit(ray), 0.0, MAXDOUBLE, record) {
        return Color3(FreeVec3(record.normal.x()+1, record.normal.y()+1, record.normal.z()+1) * 0.5);
    }
    const UnitVec3 unit_direction = ray.direction();
    const value_type t = 0.5 * (unit_direction.y() + 1.0);
    return Color3(FreeVec3(1.0, 1.0, 1.0) * (1.0 - t) + (FreeVec3(0.5, 0.7, 1.0) * t));
}



#endif //RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
