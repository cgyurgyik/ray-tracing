#ifndef RAYTRACING_ROTATEY_H
#define RAYTRACING_ROTATEY_H
#include "Hittable.h"
#include <limits>

// Rotation about the Y axis. Angle should be provided in degrees.
class RotateY : public Hittable {
public:
    RotateY(Hittable* hittable_pointer, value_type angle) : hittable_pointer_{hittable_pointer} {
        const value_type radians = (M_PI / 180.0) * angle;
        sin_theta_ = sin(radians);
        cos_theta_ = cos(radians);
        has_box_ = hittable_pointer->bounding_box(0.0, 1.0, bounding_box_);
        BoundVec3 min(std::numeric_limits<value_type>::max(),
                      std::numeric_limits<value_type>::max(),
                      std::numeric_limits<value_type>::max());
        BoundVec3 max(-std::numeric_limits<value_type>::max(),
                      -std::numeric_limits<value_type>::max(),
                      -std::numeric_limits<value_type>::max());
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    const value_type x = i * bounding_box_.max().x() + (1 - i) * bounding_box_.min().x();
                    const value_type y = j * bounding_box_.max().y() + (1 - j) * bounding_box_.min().y();
                    const value_type z = k * bounding_box_.max().z() + (1 - k) * bounding_box_.min().z();
                    const value_type new_x = cos_theta_ * x + sin_theta_ * z;
                    const value_type new_z = -sin_theta_ * x + cos_theta_ * z;

                    BoundVec3 tester(new_x, y, new_z);
                    if (tester.x() > max.x()) {
                        max.x() = tester.x();
                    }
                    if (tester.x() < min.x()) {
                        min.x() = tester.x();
                    }
                    if (tester.y() > max.y()) {
                        max.y() = tester.y();
                    }
                    if (tester.y() < min.y()) {
                        min.y() = tester.y();
                    }
                    if (tester.z() > max.z()) {
                        max.z() = tester.z();
                    }
                    if (tester.z() < min.z()) {
                        min.z() = tester.z();
                    }
                }
            }
        }
        bounding_box_ = AxisAlignedBoundingBox(min, max);
    }

    virtual bool hit(const Ray& ray, value_type t_min, value_type t_max, HitRecord& record) const {
        BoundVec3 origin = ray.origin();
        FreeVec3 direction = ray.direction().to_free();
        origin.x() = cos_theta_ * ray.origin().x() - sin_theta_ * ray.origin().y();
        origin.z() = sin_theta_ * ray.origin().x() + cos_theta_ * ray.origin().z();

        direction.x() = cos_theta_ * ray.direction().x() - sin_theta_ * ray.direction().y();
        direction.z() = sin_theta_ * ray.direction().x() + cos_theta_ * ray.direction().z();

        const Ray rotated_ray(origin, UnitVec3(direction), ray.time());
        if (hittable_pointer_->hit(rotated_ray, t_min, t_max, record)) {
            BoundVec3 point_at_parameter = record.point_at_parameter;
            FreeVec3 normal = record.normal;

            point_at_parameter.x() = cos_theta_ * record.point_at_parameter.x()
                                     + sin_theta_ * record.point_at_parameter.z();
            point_at_parameter.z() = -sin_theta_ * record.point_at_parameter.x()
                                     + cos_theta_*record.point_at_parameter.z();

            normal.x() = cos_theta_ * record.normal.x() + sin_theta_ * record.normal.z();
            normal.z() = -sin_theta_ * record.normal.x() + cos_theta_*record.normal.z();

            record.point_at_parameter = point_at_parameter;
            record.normal = normal;
            return true;
        }
        return false;
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = bounding_box_;
        return has_box_;
    }

private:
    Hittable* hittable_pointer_;
    value_type sin_theta_;
    value_type cos_theta_;
    bool has_box_;
    AxisAlignedBoundingBox bounding_box_;
};
#endif //RAYTRACING_ROTATEY_H
