#ifndef RAYTRACING_BLOCK_H
#define RAYTRACING_BLOCK_H
#include "Hittable.h"
#include "HittableList.h"
#include "Rectangle_XZ.h"
#include "Rectangle_XY.h"
#include "Rectangle_YZ.h"
#include "FlipNormals.h"
#include <vector>

// Represents an axis aligned block. Each of the 6 sides is a rectangle.
class Block : public Hittable {
public:
    Block(const BoundVec3& p0, const BoundVec3& p1, std::shared_ptr<Material> material) {
        p_min_ = p0;
        p_max_ = p1;

        hittable_list_ = std::make_unique<HittableList>(HittableList());
        hittable_list_->hittables_.push_back(std::make_unique<Rectangle_XY>(Rectangle_XY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material)));
        hittable_list_->hittables_.push_back(std::make_unique<FlipNormals>(FlipNormals(new Rectangle_XY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material))));
        hittable_list_->hittables_.push_back(std::make_unique<Rectangle_XZ>(Rectangle_XZ(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material)));
        hittable_list_->hittables_.push_back(std::make_unique<FlipNormals>(FlipNormals(new Rectangle_XZ(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material))));
        hittable_list_->hittables_.push_back(std::make_unique<Rectangle_YZ>(Rectangle_YZ(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material)));
        hittable_list_->hittables_.push_back(std::make_unique<FlipNormals>(FlipNormals(new Rectangle_YZ(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material))));

        list_pointer_ = hittable_list_.get();
    }
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        return list_pointer_->hit(ray, t0, t1, record);
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(p_min_, p_max_);
        return true;
    }
private:
    BoundVec3 p_min_;
    BoundVec3 p_max_;
    Hittable* list_pointer_;
    const int number_of_sides_ = 6;
    std::unique_ptr<HittableList> hittable_list_;
};
#endif //RAYTRACING_BLOCK_H
