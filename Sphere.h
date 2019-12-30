#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H
#include "Vec3.h"
#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(Vec3 center, value_type radius) : center_{center}, radius_{radius} {}

    bool hit(const Ray& ray, value_type minimum, value_type maximum, hit_record& record) {

    }
private:
    const Vec3 center_;
    const value_type radius_;
};
#endif //RAYTRACING_SPHERE_H
