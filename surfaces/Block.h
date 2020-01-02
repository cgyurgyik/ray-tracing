#ifndef RAYTRACING_BLOCK_H
#define RAYTRACING_BLOCK_H
#include "Hittable.h"
#include "HittableList.h"
#include "Rectangle_XZ.h"
#include "Rectangle_XY.h"
#include "Rectangle_YZ.h"
#include "FlipNormals.h"

// Represents an axis aligned block. Each of the 6 sides is a rectangle.
class Block : public Hittable {
public:
    Block(const BoundVec3& p0, const BoundVec3& p1, Material* material_pointer) {
        p_min_ = p0;
        p_max_ = p1;
        Hittable** list = new Hittable*[number_of_sides_]; // 6 sides.
        list[0] = new Rectangle_XY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material_pointer);
        list[1] = new FlipNormals(new Rectangle_XY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material_pointer));
        list[2] = new Rectangle_XZ(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material_pointer);
        list[3] = new FlipNormals(new Rectangle_XZ(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material_pointer));
        list[4] = new Rectangle_YZ(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material_pointer);
        list[5] = new FlipNormals(new Rectangle_YZ(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material_pointer));

        list_pointer_ = new HittableList(list, number_of_sides_);
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
};
#endif //RAYTRACING_BLOCK_H
