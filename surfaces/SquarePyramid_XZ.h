#ifndef RAYTRACING_SQUAREPYRAMID_XZ_H
#define RAYTRACING_SQUAREPYRAMID_XZ_H

#include "Hittable.h"
#include "HittableWorld.h"
#include "Triangle.h"
#include "Rectangle_XZ.h"
#include "transformations/RotateY.h"
#include "FlipNormals.h"
#include "transformations/Translate.h"

// Represents a square pyramid with base in the XZ plane.
class SquarePyramid_XZ : public Hittable {
public:
    SquarePyramid_XZ(const BoundVec3& base, int height, std::shared_ptr<const Material> material) :
            height_{height} {
        base_ = base;

        const value_type base_x = base_.x();
        const value_type base_y = base_.y();
        const value_type base_z = base_.z();

        const auto bottom = std::make_shared<FlipNormals>
                (FlipNormals(std::make_shared<Rectangle_XZ>(
                        Rectangle_XZ(base_x, base_x * 2,
                                     base_z, base_z * 2,
                                     base_y, material))));

        const BoundVec3 front_left(2.0 * base_x, base_y, base_z);
        const BoundVec3 front_right(base_x, base_y, base_z);
        const BoundVec3 front_middle(1.5 * base_x, height_, 1.0 * base_z);
        const auto front_triangle = std::make_shared<Triangle>
                (Triangle(front_left,
                          front_right,
                          front_middle,
                          material));

        const BoundVec3 back_left(2.0 * base_x, 1.5 * base_y, 2.0 * base_z);
        const BoundVec3 back_right(base_x, 1.5 * base_y, 2.0 * base_z);
        const BoundVec3 back_middle(1.5 * base_x, height_, 2.0 * base_z);
        const auto back_triangle = std::make_shared<Triangle>
                (Triangle(back_left,
                          back_right,
                          back_middle,
                          material));

        const BoundVec3 l_left(2.0 * base_x, 1.5 * base_y, 2.0 * base_z);
        const BoundVec3 l_right(2.0 * base_x, base_y, base_z);
        const BoundVec3 l_middle(2.0 * base_x, height_, 1.5 * base_z);
        const auto l_triangle = std::make_shared<Triangle>
                (Triangle(l_left,
                          l_right,
                          l_middle,
                          material));

        const BoundVec3 r_left(base_x, base_y, base_z);
        const BoundVec3 r_right(base_x, 1.5 * base_y, 2.0 * base_z);
        const BoundVec3 r_middle(1.0 * base_x, height_, 1.5 * base_z);
        const auto r_triangle = std::make_shared<Triangle>
                (Triangle(r_left,
                          r_right,
                          r_middle,
                          material));

        hittable_list_ = std::make_unique<HittableWorld>(HittableWorld(/*size=*/5));
        hittable_list_->add(bottom);
        //hittable_list_->add(front_triangle);
        //hittable_list_->add(std::make_unique<FlipNormals>(FlipNormals(back_triangle)));
        hittable_list_->add(l_triangle);
        hittable_list_->add(r_triangle);
        pyramid_pointer_ = hittable_list_.get();
    }
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        return pyramid_pointer_->hit(ray, t0, t1, record);
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(base_, base_ + FreeVec3(height_, height_, height_));
        return true;
    }
private:
    BoundVec3 base_;
    const int height_;
    Hittable* pyramid_pointer_;
    std::unique_ptr<HittableWorld> hittable_list_;
};

#endif //RAYTRACING_SQUAREPYRAMID_XZ_H