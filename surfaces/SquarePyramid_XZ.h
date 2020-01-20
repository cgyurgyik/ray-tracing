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
// It contains one square and four triangles.
class SquarePyramid_XZ : public Hittable {
public:
    SquarePyramid_XZ(const BoundVec3& base, int height, std::shared_ptr<const Material> material) :
            height_{height} {
        base_ = base;
        const value_type base_x = base_.x();
        const value_type base_y = base_.y();
        const value_type base_z = base_.z();
        const BoundVec3 middle(1.5 * base_x, height_, 1.5 * base_z);

        const auto bottom = std::make_shared<FlipNormals>
                (FlipNormals(std::make_shared<Rectangle_XZ>(
                        Rectangle_XZ(base_x, base_x * 2.0,
                                     base_z, base_z * 2.0,
                                     base_y, material))));

        const auto front_triangle = std::make_shared<Triangle>
                (Triangle(BoundVec3(2.0 * base_x, base_y, base_z),
                          BoundVec3(base_x, base_y, base_z),
                          middle, material));

        const auto back_triangle = std::make_shared<FlipNormals>(FlipNormals(std::make_shared<Triangle>
                (Triangle(BoundVec3(2.0 * base_x, 1.5 * base_y, 2.0 * base_z),
                          BoundVec3(base_x, 1.5 * base_y, 2.0 * base_z),
                          middle, material))));

        const auto l_triangle = std::make_shared<Triangle>
                (Triangle(BoundVec3(2.0 * base_x, 1.5 * base_y, 2.0 * base_z),
                          BoundVec3(2.0 * base_x, base_y, base_z),
                          middle, material));

        const auto r_triangle = std::make_shared<Triangle>
                (Triangle(BoundVec3(base_x, base_y, base_z),
                          BoundVec3(base_x, 1.5 * base_y, 2.0 * base_z),
                          middle, material));

        hittable_list_ = std::make_unique<HittableWorld>(HittableWorld(/*size=*/5));
        hittable_list_->add(bottom);
        hittable_list_->add(front_triangle);
        hittable_list_->add(back_triangle);
        hittable_list_->add(l_triangle);
        hittable_list_->add(r_triangle);
        square_pyramid_pointer_ = hittable_list_.get();
    }
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const override {
        return square_pyramid_pointer_->hit(ray, t0, t1, record);
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const override {
        box = AxisAlignedBoundingBox(base_, base_ + FreeVec3(height_, height_, height_));
        return true;
    }
private:
    // The base of the pyramid.
    BoundVec3 base_;
    // The height of the pyramid.
    const int height_;
    // A pointer to the square pyramid.
    Hittable* square_pyramid_pointer_;
    // A list containing each component of the square pyramid.
    std::unique_ptr<HittableWorld> hittable_list_;
};

#endif //RAYTRACING_SQUAREPYRAMID_XZ_H