#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include <vector>
#include <memory>
#include "../utility/Vec3.h"
#include "../surfaces/Hittable.h"
#include "../surfaces/HittableWorld.h"
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

// Represents a scene. The world contains the hittables, and the camera contains the necessary angles and times.
struct Scene {
    std::unique_ptr<Camera> camera;
    std::unique_ptr<HittableWorld> world;
    int maximum_depth;
    // TODO: If no light is provided, add background
};

// Creates the Cornell Box. Aspect is determined by the ('x_pixels' / 'y_pixels').
Scene cornell_box(int x_pixels, int y_pixels, int maximum_depth) {
    // Positionable camera.
    const BoundVec3 look_from(278.0, 278.0, -800.0);
    const FreeVec3 look_at(278.0, 278.0, 0.0);
    const FreeVec3 view_up(0.0, 1.0, 0.0);
    const value_type distance_to_focus = 10.0;
    const value_type aperture = 0.0;
    const value_type field_of_view = 40.0;
    const value_type time0 = 0.0;
    const value_type time1 = 1.0;
    const value_type aspect = value_type(x_pixels)/value_type(y_pixels);
    std::unique_ptr<Camera> current_camera = std::make_unique<Camera>(Camera(look_from, look_at, view_up, field_of_view, aspect,
                        aperture, distance_to_focus, time0, time1));

    // World.
    std::unique_ptr<HittableWorld> hittable_list = std::make_unique<HittableWorld>(HittableWorld());

    const auto red_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.65, 0.05, 0.05)))));
    const auto white_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.73, 0.73, 0.73)))));
    const auto green_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.12, 0.45, 0.15)))));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)))));

    // Left wall.
    const auto left_wall = std::make_shared<Rectangle_YZ>(Rectangle_YZ(0, 555, 0, 555, 555, green_material));
    hittable_list->add(std::make_shared<FlipNormals>(FlipNormals(left_wall)));

    // Right wall.
    const auto right_wall = std::make_shared<Rectangle_YZ>(Rectangle_YZ(0, 555, 0, 555, 0, red_material));
    hittable_list->add(right_wall);

    // Light source.
    const auto light_source = std::make_shared<Rectangle_XZ>(Rectangle_XZ(113, 443, 127, 432, 554, light));
    hittable_list->add(light_source);

    // Ceiling.
    const auto ceiling = std::make_shared<Rectangle_XZ>(Rectangle_XZ(0, 555, 0, 555, 555, white_material));
    hittable_list->add(std::make_shared<FlipNormals>(FlipNormals(ceiling)));

    // Floor.
    const auto floor = std::make_shared<Rectangle_XZ>(Rectangle_XZ(0, 555, 0, 555, 0, white_material));
    hittable_list->add(floor);

    // Back wall.
    const auto back_wall = std::make_shared<Rectangle_XY>(Rectangle_XY(0, 555, 0, 555, 555, white_material));
    hittable_list->add(std::make_shared<FlipNormals>(FlipNormals(back_wall)));

    // Left block.
    const auto left_block = std::make_shared<Block>(Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 165.0, 165.0), white_material));
    const auto left_block_offset = FreeVec3(130.0, 0.0, 65.0);
    const auto left_block_rotation = std::make_shared<RotateY>(RotateY(left_block, /*angle_in_degrees=*/-18.0));
    hittable_list->add(std::make_shared<Translate>(Translate(left_block_rotation, left_block_offset)));

    // Right block.
    const auto right_block = std::make_shared<Block>(Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 330.0, 165.0), white_material));
    const auto right_block_offset = FreeVec3(265.0, 0.0, 295.0);
    const auto right_block_rotation = std::make_shared<RotateY>(RotateY(right_block, /*angle_in_degrees=*/15.0));
    hittable_list->add(std::make_shared<Translate>(Translate(right_block_rotation, right_block_offset)));

    return Scene{.camera=std::move(current_camera), .world=std::move(hittable_list), .maximum_depth=maximum_depth};
}

#endif //RAYTRACING_SCENE_H
