#ifndef RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
#define RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
#include "../utility/Vec3.h"
#include "../utility/Ray.h"

// Avoids unnecessary checks such as NaN.
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

    bool hit(const Ray& ray, value_type t_min, value_type t_max) const {
        const FreeVec3 direction = ray.direction().to_free();
        for (int axis = 0; axis < 3; ++axis) {
            const value_type invD = 1.0 / direction[axis];
            value_type t0 = (min()[axis] - ray.origin()[axis]) * invD;
            value_type t1 = (max()[axis] - ray.origin()[axis]) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min) return false;
        }
        return true;
    }

private:
    BoundVec3 min_;
    BoundVec3 max_;
};
#endif //RAYTRACING_AXISALIGNEDBOUNDINGBOX_H
