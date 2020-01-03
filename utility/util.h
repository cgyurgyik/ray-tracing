#ifndef RAYTRACING_UTIL_H
#define RAYTRACING_UTIL_H
#include "../surfaces/HittableList.h"
#include "../surfaces/Sphere.h"
#include "../surfaces/Hittable.h"
#include <limits>
#include <functional>
#include <random>
#include "../material/Material.h"


// Generates a pseudorandom number between 0.0 and 1.0.
// See: <random> for more information.
inline value_type random_value() {
    static std::uniform_real_distribution<value_type> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<value_type()> rand_generator =
            std::bind(distribution, generator);
    return rand_generator();
}

// Generates a random value in a unit disk, where
// x, y, are bounded by [-1, 1] and z = 0.
FreeVec3 random_value_in_unit_disk() {
    FreeVec3 v;
    do {
        v = FreeVec3(random_value(), random_value(), 0) * 2.0 - FreeVec3(1.0, 1.0, 0.0);
    } while (v.dot(v) >= 1.0);
    return v;
}

// Generates a random value in a unit sphere, where
// x, y, z are bounded by [-1, 1].
FreeVec3 random_value_in_unit_sphere() {
    FreeVec3 v;
    do {
        v = FreeVec3(random_value(), random_value(), random_value()) * 2.0 - FreeVec3(1.0, 1.0, 1.0);
    } while (v.dot(v) >= 1.0);
    return v;
}

// TODO: Documentation, depth magic number.
Color3 color(const Ray& ray, Hittable *world, int depth) {
    HitRecord record;
    const bool is_world_hit = world->hit(ray, /*minimum=*/value_type(0.001),
            /*maximum=*/std::numeric_limits<value_type>::max(), record);
    if (is_world_hit) {
        Ray scattered;
        Color3 attenuation;
        const Color3 emitted_light = record.material->emitted(record.u, record.v, record.point_at_parameter);
        const bool meets_depth_check = depth < 50;  /*MAGIC NUMBER*/
        if (meets_depth_check && record.material->scatter(ray, record, attenuation, scattered)) {
            return emitted_light + (attenuation * color(scattered, world, depth + 1));
        }
        return emitted_light;
    }
    return Color3(0.0, 0.0, 0.0);
}

#endif //RAYTRACING_UTIL_H
