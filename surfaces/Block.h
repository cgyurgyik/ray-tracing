#ifndef RAYTRACING_BLOCK_H
#define RAYTRACING_BLOCK_H
#include "Hittable.h"
#include "HittableWorld.h"
#include "Rectangle_XZ.h"
#include "Rectangle_XY.h"
#include "Rectangle_YZ.h"
#include "FlipNormals.h"

// Represents an axis aligned block. Each of the 6 sides is a rectangle.
// Currently, they will all share the same material.
class Block : public Hittable {
public:
    Block(const BoundVec3& p0, const BoundVec3& p1, std::shared_ptr<const Material> material) {
        p_min_ = p0;
        p_max_ = p1;

        const value_type p0_x = p0.x();
        const value_type p0_y = p0.y();
        const value_type p0_z = p0.z();
        const value_type p1_x = p1.x();
        const value_type p1_y = p1.y();
        const value_type p1_z = p1.z();

        const auto front = std::make_shared<Rectangle_XY>
                (Rectangle_XY(p0_x, p1_x, p0_y, p1_y, p0_z, material));
        const auto back = std::make_shared<FlipNormals>
        (FlipNormals(std::make_shared<Rectangle_XY>(Rectangle_XY(p0_x, p1_x, p0_y, p1_y, p0_z, material))));
        const auto top = std::make_shared<FlipNormals>
                (FlipNormals(std::make_shared<Rectangle_XZ>(Rectangle_XZ(p0_x, p1_x, p0_z, p1_z, p0_y, material))));
        const auto bottom = std::make_shared<Rectangle_XZ>
                (Rectangle_XZ(p0_x, p1_x, p0_z, p1_z, p1_y, material));
        const auto right = std::make_shared<Rectangle_YZ>
        (Rectangle_YZ(p0_y, p1_y, p0_z, p1_z, p1_x, material));
        const auto left = std::make_shared<FlipNormals>
        (FlipNormals(std::make_shared<Rectangle_YZ>(Rectangle_YZ(p0_y, p1_y, p0_z, p1_z, p0_x, material))));

        hittable_list_ = std::make_unique<HittableWorld>(HittableWorld(/*size=*/6));
        hittable_list_->add(front);
        hittable_list_->add(back);
        hittable_list_->add(top);
        hittable_list_->add(bottom);
        hittable_list_->add(right);
        hittable_list_->add(left);

        block_pointer_ = hittable_list_.get();
    }
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        return block_pointer_->hit(ray, t0, t1, record);
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(p_min_, p_max_);
        return true;
    }
private:
    BoundVec3 p_min_;
    BoundVec3 p_max_;
    Hittable* block_pointer_;
    std::unique_ptr<HittableWorld> hittable_list_;
};
#endif //RAYTRACING_BLOCK_H
