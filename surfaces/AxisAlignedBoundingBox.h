#ifndef RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
#define RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
#include "../utility/Vec3.h"
#include "../utility/Ray.h"

inline value_type get_min(value_type a, value_type b) { return a < b ? a : b; }
inline value_type get_max(value_type a, value_type b) { return a > b ? a : b; }

class AxisAlignedBoundingBox {
public:

    AxisAlignedBoundingBox() {}
    AxisAlignedBoundingBox(const BoundVec3& min, const BoundVec3& max) : min_{min}, max_{max} {}

    BoundVec3 min() const { return min_; }
    BoundVec3 max() const { return max_; }

    static AxisAlignedBoundingBox surrounding_box(const AxisAlignedBoundingBox& box0, const AxisAlignedBoundingBox& box1) {
        const BoundVec3 small(get_min(box0.min().x(), box1.min().x()),
                              get_min(box0.min().y(), box1.min().y()),
                              get_min(box0.min().z(), box1.min().z()));
        const BoundVec3 large(get_max(box0.max().x(), box1.max().x()),
                              get_max(box0.max().y(), box1.max().y()),
                              get_max(box0.max().z(), box1.max().z()));
        return AxisAlignedBoundingBox(small, large);
    }

    // TODO: Update using Andrew Kensler's faster method which incorporates std::swap().
    bool hit(const Ray& ray, value_type t_min, value_type t_max) const {
        const value_type inv_x_direction  = 1.0 / ray.direction().x();
        const value_type x_min = get_min(min_.x() - ray.origin().x() * inv_x_direction,
                max_.x() - ray.origin().x() * inv_x_direction);
        const value_type x_max = get_max(min_.x() - ray.origin().x() / ray.direction().x(),
                                         max_.x() - ray.origin().x() / ray.direction().x());

        t_min = get_max(x_min, t_min);
        t_max = get_min(x_max, t_max);
        if (t_max <= t_min) return false;

        const value_type inv_y_direction  = 1.0 / ray.direction().y();
        const value_type y_min = get_min(min_.y() - ray.origin().y() * inv_y_direction,
                                         max_.y() - ray.origin().y() * inv_y_direction);
        const value_type y_max = get_max(min_.y() - ray.origin().y() * inv_y_direction,
                                         max_.y() - ray.origin().y() * inv_y_direction);

        t_min = get_max(y_min, t_min);
        t_max = get_min(y_max, t_max);
        if (t_max <= t_min) return false;

        const value_type inv_z_direction  = 1.0 / ray.direction().z();
        const value_type z_min = get_min(min_.z() - ray.origin().z() * inv_z_direction,
                                         max_.z() - ray.origin().z() * inv_z_direction);
        const value_type z_max = get_max(min_.z() - ray.origin().z() * inv_z_direction,
                                         max_.z() - ray.origin().z() * inv_z_direction);

        t_min = get_max(z_min, t_min);
        t_max = get_min(z_max, t_max);
        return (t_max <= t_min);
    }

private:
    BoundVec3 min_;
    BoundVec3 max_;
};
#endif //RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
