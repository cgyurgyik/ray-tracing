#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include <vector>
#include <memory>
#include "../utility/Vec3.h"
#include "../surfaces/Hittable.h"
#include "../surfaces/HittableList.h"
#include "../surfaces/Sphere.h"
#include "../surfaces/Rectangle_XY.h"
#include "../surfaces/Rectangle_XZ.h"
#include "../surfaces/Rectangle_YZ.h"
#include "../surfaces/RotateY.h"
#include "../surfaces/Translate.h"
#include "../surfaces/Block.h"
#include "../surfaces/FlipNormals.h"
#include "../utility/Camera.h"
#include "../material/Lambertian.h"
#include "../material/Metal.h"
#include "../material/Dielectric.h"
#include "../material/ConstantTexture.h"
#include "../material/CheckerTexture.h"
#include "../material/DiffuseLight.h"


// A number of demonstrations using the provided materials and surfaces.
// TODO: Eventually, want to create a scene object that also accounts for
//       Camera angles, objects, and background.

// Creates the Cornell box.
//
//    Positionable camera angles listed below:
//    const BoundVec3 look_from(278.0, 278.0, -800.0);
//    const FreeVec3 look_at(278.0, 278.0, 0.0);
//    const FreeVec3 view_up(0.0, 1.0, 0.0);
//    const value_type distance_to_focus = 1.0;
//    const value_type aperture = 0.0;
//    const value_type field_of_view = 40.0;
//    const value_type aspect = value_type(x_pixels)/value_type(y_pixels);
//    const value_type time0 = 0.0;
//    const value_type time1 = 1.0;
std::unique_ptr<HittableList> cornell_box() {
    std::unique_ptr<HittableList> hittable_list = std::make_unique<HittableList>(HittableList());

    const auto red_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.65, 0.05, 0.05)))));
    const auto white_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.73, 0.73, 0.73)))));
    const auto green_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.12, 0.45, 0.15)))));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)))));

    // Left wall.
    hittable_list->hittables_.push_back(std::make_unique<FlipNormals>(FlipNormals(new Rectangle_YZ(0, 555, 0, 555, 555, green_material))));

    // Right wall.
    hittable_list->hittables_.push_back(std::make_unique<Rectangle_YZ>(Rectangle_YZ(0, 555, 0, 555, 0, red_material)));

    // Light source.
    hittable_list->hittables_.push_back(std::make_unique<Rectangle_XZ>(Rectangle_XZ(113, 443, 127, 432, 554, light)));

    // Ceiling.
    hittable_list->hittables_.push_back(std::make_unique<Rectangle_XZ>(Rectangle_XZ(0, 555, 0, 555, 555, white_material)));

    // Floor.
    hittable_list->hittables_.push_back(std::make_unique<Rectangle_XZ>(Rectangle_XZ(0, 555, 0, 555, 0, white_material)));

    // Back wall.
    hittable_list->hittables_.push_back(std::make_unique<FlipNormals>(FlipNormals(new Rectangle_XY(0, 555, 0, 555, 555, white_material))));

    // Left block.
    const auto left_block = new Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 165.0, 165.0), white_material);
    const auto left_block_offset = FreeVec3(130.0, 0.0, 65.0);
    const value_type left_block_rotation = -18.0;
    hittable_list->hittables_.push_back(std::make_unique<Translate>(Translate(new RotateY(left_block, left_block_rotation), left_block_offset)));

    // Right block.
    const auto right_block = new Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 330.0, 165.0), white_material);
    const auto right_block_offset = FreeVec3(265.0, 0.0, 295.0);
    const value_type right_block_rotation = 15.0;
    hittable_list->hittables_.push_back(std::make_unique<Translate>(Translate(new RotateY(right_block, right_block_rotation), right_block_offset)));

    return hittable_list;
}

#endif //RAYTRACING_SCENE_H
