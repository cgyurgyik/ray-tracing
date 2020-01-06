#ifndef RAYTRACING_TETRAHEDRON_H
#define RAYTRACING_TETRAHEDRON_H

#include "Hittable.h"
#include "HittableWorld.h"
#include "Triangle.h"
#include "Rectangle_XZ.h"
#include "FlipNormals.h"

// Represents a tetrahedron.
// TODO: Probably re-label as XZ tetrahedron.
class Tetrahedron : public Hittable {
public:
    Tetrahedron(const BoundVec3& base, int height, std::shared_ptr<const Material> material) :
    height_{height} {
        base_ = base;

        const value_type base_x = base_.x();
        const value_type base_y = base_.y();
        const value_type base_z = base_.z();

        const auto bottom = std::make_shared<FlipNormals>
                (FlipNormals(std::make_shared<Rectangle_XZ>(Rectangle_XZ(base_x, base_x * 2, base_z, base_z * 2, base_y, material))));
        const auto triangle = std::make_shared<Triangle>
               (Triangle(BoundVec3(base_x * 2, base_y, base_z),
                         BoundVec3(base_x, base_y, base_z * 2),
                         BoundVec3(base_x + base_x * 1/2, height_, base_z + base_z * 1/2),
                         material));

        hittable_list_ = std::make_unique<HittableWorld>(HittableWorld(/*size=*/5));
        hittable_list_->add(bottom);
        hittable_list_->add(triangle);

        tetrahedron_pointer_ = hittable_list_.get();
    }
    virtual bool hit(const Ray& ray, value_type t0, value_type t1, HitRecord& record) const {
        return tetrahedron_pointer_->hit(ray, t0, t1, record);
    }

    virtual bool bounding_box(value_type t0, value_type t1, AxisAlignedBoundingBox& box) const {
        box = AxisAlignedBoundingBox(base_, base_ + FreeVec3(height_, height_, height_));
        return true;
    }
private:
    BoundVec3 base_;
    const int height_;
    Hittable* tetrahedron_pointer_;
    std::unique_ptr<HittableWorld> hittable_list_;
};

#endif //RAYTRACING_TETRAHEDRON_H
