#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "Vec3.h"
#include <cmath>
#include "util.h"

// Encapsulates a positionable camera.
// Note, while this camera will use radians for calculations,
// the inputs should be in degrees for simplicity.
class Camera {
public:
    // The field_of_view is the value from top to bottom in degrees.
    // The half height is then calculated by tan(field_of_view/2),
    // and then half width is calculated by multiplying this by the aspect.
    // From there, we can produce our horizontal, vertical, and lower left corner vectors.
    Camera(const BoundVec3& look_from, const FreeVec3& look_at, const FreeVec3& view_up,
            value_type field_of_view, value_type aspect, value_type aperture, value_type focus_distance,
            value_type t0, value_type t1) :
            field_of_view_{field_of_view}, aspect_{aspect}, time0_{t0}, time1_{t1} {

        lens_radius_ = aperture / 2.0;
        origin_ = look_from;
        w_ = UnitVec3(look_from - look_at);
        u_ = UnitVec3(view_up.cross(w_.to_free()));
        v_ = w_.to_free().cross(u_.to_free());

        const value_type theta = field_of_view * M_PI/180;
        const value_type half_height = tan(theta / 2.0);
        const value_type half_width = aspect * half_height;

        lower_left_corner_ = BoundVec3(origin_
                - (u_ * half_width * focus_distance)
                - (v_ * half_height * focus_distance)
                - w_.to_free() * focus_distance);
        horizontal_ = FreeVec3(u_.to_free() * 2 * half_width * focus_distance);
        vertical_ = FreeVec3(v_ * 2 * half_height * focus_distance);
    }

    // Gets the current ray from the camera point of view.
    Ray getRay(value_type s, value_type t) const {
        const FreeVec3 rd = random_value_in_unit_disk() * lens_radius_;
        const FreeVec3 offset = u_ * rd.x() + v_ * rd.y();
        const value_type time = time0_ + random_value() * (time1_ - time0_);
        return Ray(origin_ + offset,
                UnitVec3(lower_left_corner_
                + (horizontal_ * s)
                + (vertical_ * t)
                - origin_
                - offset));
    }

    // Dampens the current color by square-rooting each value.
    static inline void dampen(Color3& current_color) {
        current_color = Color3(std::sqrt(current_color.r()),
                      std::sqrt(current_color.g()),
                      std::sqrt(current_color.b()));
    }

    // To implement anti-aliasing, take the average sample over n trials.
    // This allows for less jagged edges for each surface in the world.
    static void antialiasing(Color3& current_color, Camera* camera, HittableWorld* world,
                      int num_runs, int x_pixels, int y_pixels, int i, int j,
                      int maximum_recursion_depth) {
        for (int current_run = 0; current_run < num_runs; ++current_run) {
            const value_type u = value_type(i + random_value()) / value_type(x_pixels);
            const value_type v = value_type(j + random_value()) / value_type(y_pixels);
            const Ray ray = camera->getRay(u, v);
            int current_recursion_depth = 0;
            current_color += color(ray, world,  maximum_recursion_depth, current_recursion_depth);
        }
        current_color /= value_type(num_runs); // Take average sample.
    }

private:
    const value_type field_of_view_;
    const value_type aspect_;
    value_type lens_radius_;
    BoundVec3 origin_;
    BoundVec3 lower_left_corner_;
    FreeVec3 horizontal_;
    FreeVec3 vertical_;
    // u_, v_, and w_ are used to produce an orthonormal basis
    // to describe the camera's orientation.
    UnitVec3 u_, w_;
    FreeVec3 v_;
    value_type time0_, time1_;
};
#endif //RAYTRACING_CAMERA_H
