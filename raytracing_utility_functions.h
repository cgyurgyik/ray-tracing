#ifndef RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#define RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H
#include "Vec3.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Hittable.h"
#include <limits>
#include <functional>
#include <random>

// Generates a pseudorandom number between 0.0 and 1.0.
// See: <random> for more information.
inline value_type random_value() {
    static std::uniform_real_distribution<value_type> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<value_type()> rand_generator =
            std::bind(distribution, generator);
    return rand_generator();
}

// Generates a random value in a unit sphere, where
// x, y, z are bounded by [-1, 1].
FreeVec3 random_value_in_unit_sphere() {
    FreeVec3 v(random_value(), random_value(), random_value());
    while (v.squared_length() < 1.0) {
        v = FreeVec3(random_value(), random_value(), random_value()) * 2.0;
    }
    return v;
}

// TODO: Fix.
Color3 color(const Ray& ray, Hittable *world) {
    HitRecord record;
    const bool is_world_hit = world->hit(ray, /*minimum=*/value_type(0.001),
            /*maximum=*/std::numeric_limits<value_type>::max(), record);
    if (is_world_hit) {
        const BoundVec3 point_at_parameter = record.point_at_parameter;
        const BoundVec3 target = point_at_parameter
                + record.normal + random_value_in_unit_sphere();
        const Ray new_ray(point_at_parameter,
                UnitVec3(target - point_at_parameter));
        return color(new_ray, world) * 0.5;
    }
    const UnitVec3 unit_direction = ray.direction();
    const value_type t = 0.5 * (unit_direction.y() + 1.0);
    return Color3(1.0, 1.0, 1.0) * (1.0 - t) + Color3(0.5, 0.7, 1.0) * t;
}

// Dampens the current color by square-rooting each value.
inline Color3 dampen(const Color3& current_color) {
    return Color3(std::sqrt(current_color.r()),
                  std::sqrt(current_color.g()),
                  std::sqrt(current_color.b()));
}

// To implement anti-aliasing, take the average sample over n trials.
// This allows for less jagged edges for each surface in the world.
void antialiasing(Color3& current_color, const Camera& camera, HittableList* world,
        int num_runs, int x_pixels, int y_pixels, int i, int j) {
    for (int current_run = 0; current_run < num_runs; ++current_run) {
        const value_type u = value_type(i) + random_value() / value_type(x_pixels);
        const value_type v = value_type(j) + random_value() / value_type(y_pixels);
        const Ray ray = camera.getRay(u, v);
        current_color += color(ray, world);
        current_color /= value_type(num_runs);
    }
}

#endif //RAYTRACING_RAYTRACING_UTILITY_FUNCTIONS_H