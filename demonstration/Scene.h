#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include "../utility/Vec3.h"
#include "../surfaces/Hittable.h"
#include "../surfaces/HittableWorld.h"
#include "../surfaces/Sphere.h"
#include "../surfaces/Rectangle_XY.h"
#include "../surfaces/Rectangle_XZ.h"
#include "../surfaces/Rectangle_YZ.h"
#include "../surfaces/Triangle.h"
#include "../surfaces/SquarePyramid_XZ.h"
#include "../surfaces/transformations/RotateY.h"
#include "../surfaces/transformations/RotateX.h"
#include "../surfaces/transformations/RotateZ.h"
#include "../surfaces/transformations/Translate.h"
#include "../surfaces/Block.h"
#include "../surfaces/FlipNormals.h"
#include "../utility/Camera.h"
#include "../material/Lambertian.h"
#include "../material/Metal.h"
#include "../material/Dielectric.h"
#include "../material/texture/ConstantTexture.h"
#include "../material/texture/CheckerTexture.h"
#include "../material/texture/NoiseTexture.h"
#include "../material/DiffuseLight.h"
#include "../material/texture/Perlin.h"

// Represents a scene. The world contains the hittables, and the camera contains the necessary angles and times.
struct Scene {
    // The camera used for the current scene.
    std::unique_ptr<const Camera> camera;
    // The necessary surfaces for the current scene that produces the "world".
    std::unique_ptr<const HittableWorld> world;
    // The maximum allowable recursion depth for coloring.
    int maximum_recursion_depth;
    // TODO: If no light is provided, add background
};

// Creates the Cornell Box. Aspect is determined by the ('x_pixels' / 'y_pixels').
Scene cornell_box(int x_pixels, int y_pixels, int maximum_recursion_depth) {
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
    auto current_camera = std::make_unique<Camera>(Camera(look_from, look_at, view_up, field_of_view, aspect,
                        aperture, distance_to_focus, time0, time1));

    // World.
    const int num_hittables = 8;
    auto hittable_list = std::make_unique<HittableWorld>(HittableWorld(num_hittables));

    const auto red_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.65, 0.05, 0.05)));
    const auto white_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.73, 0.73, 0.73)));
    const auto green_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.12, 0.45, 0.15)));
    const auto light_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)));

    const auto red_material = std::make_shared<Lambertian>(Lambertian(red_texture));
    const auto white_material = std::make_shared<Lambertian>(Lambertian(white_texture));
    const auto green_material = std::make_shared<Lambertian>(Lambertian(green_texture));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(light_texture));

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

    // Right block.
    const auto right_block = std::make_shared<Block>(Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 165.0, 165.0),
                                                    white_material));
    const auto right_block_offset = FreeVec3(130.0, 0.0, 65.0);
    const auto right_block_rotation = std::make_shared<RotateY>(RotateY(right_block, /*angle_in_degrees=*/-18.0));
    hittable_list->add(std::make_shared<Translate>(Translate(right_block_rotation, right_block_offset)));

    // Left block.
    const auto left_block = std::make_shared<Block>(Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 330.0, 165.0),
                                                     white_material));
    const auto left_block_offset = FreeVec3(265.0, 0.0, 295.0);
    const auto left_block_rotation = std::make_shared<RotateY>(RotateY(right_block, /*angle_in_degrees=*/15.0));
    hittable_list->add(std::make_shared<Translate>(Translate(left_block_rotation, left_block_offset)));

    return Scene{.camera=std::move(current_camera),
                 .world=std::move(hittable_list),
                 .maximum_recursion_depth=maximum_recursion_depth};
}

// Demonstrates Perlin noise on a sphere.
Scene perlin_noise_demonstration(int x_pixels, int y_pixels, int maximum_recursion_depth) {
    // Positionable camera.
    const BoundVec3 look_from(24.0, 2.0, 3.0);
    const FreeVec3 look_at(0.0, 0.0, 0.0);
    const FreeVec3 view_up(0.0, 1.0, 0.0);
    const value_type distance_to_focus = 10.0;
    const value_type aperture = 0.0;
    const value_type field_of_view = 20.0;
    const value_type time0 = 0.0;
    const value_type time1 = 1.0;
    const value_type aspect = value_type(x_pixels)/value_type(y_pixels);
    auto current_camera = std::make_unique<Camera>(Camera(look_from, look_at, view_up, field_of_view, aspect,
                                                          aperture, distance_to_focus, time0, time1));

    // World.
    const auto light_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(light_texture));
    const auto pertext_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<NoiseTexture>(
            NoiseTexture(/*scale=*/4, /*turbulence_depth=*/7, Perlin(/*num_permutations=*/256)))));

    const int num_hittables = 3;
    auto hittable_list = std::make_unique<HittableWorld>(HittableWorld(num_hittables));

    // Sphere.
    hittable_list->add(std::make_shared<Sphere>(Sphere(BoundVec3(0.0, 2.0, 0.0), 2.0,
                                                       pertext_material)));

    // Floor.
    hittable_list->add(std::make_shared<Sphere>(Sphere(BoundVec3(0.0, -1000.0, 0.0), 1000.0,
                                                       pertext_material)));

    // Rectangular light source.
    const auto rectangular_light = std::make_shared<Rectangle_XY>(Rectangle_XY(3.0, 5.0, 1.0, 3.0, -2.0, light));
    hittable_list->add(rectangular_light);

    return Scene{.camera=std::move(current_camera),
            .world=std::move(hittable_list),
            .maximum_recursion_depth=maximum_recursion_depth};
}

// A twist on the Cornell Box. Aspect is determined by the ('x_pixels' / 'y_pixels').
Scene cornell_box_twist(int x_pixels, int y_pixels, int maximum_recursion_depth) {
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
    auto current_camera = std::make_unique<Camera>(Camera(look_from, look_at, view_up, field_of_view, aspect,
                                                          aperture, distance_to_focus, time0, time1));

    // World.
    const int num_hittables = 8;
    auto hittable_list = std::make_unique<HittableWorld>(HittableWorld(num_hittables));

    const auto red_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.65, 0.05, 0.05)));
    const auto white_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.73, 0.73, 0.73)));
    const auto green_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(0.12, 0.45, 0.15)));
    const auto light_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)));

    const auto pertext_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<NoiseTexture>(
            NoiseTexture(/*scale=*/4, /*turbulence_depth=*/7, Perlin(/*num_permutations=*/256)))));
    const auto red_material = std::make_shared<Lambertian>(Lambertian(red_texture));
    const auto white_material = std::make_shared<Lambertian>(Lambertian(white_texture));
    const auto green_material = std::make_shared<Lambertian>(Lambertian(green_texture));
    const auto metal = std::make_shared<Metal>(Metal(Color3(0.8, 0.85, 0.88), 0.0));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(light_texture));

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

    // SquarePyramid_XZ
    const auto base = BoundVec3(130.0, 0.0, 100.0);
    const int height = 150;
    const auto square_pyramid = std::make_shared<SquarePyramid_XZ>(SquarePyramid_XZ(base, height, pertext_material));
    hittable_list->add(std::make_shared<RotateY>(RotateY(square_pyramid, -18.0)));

    // Left block.
    const auto left_block = std::make_shared<Block>(Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 330.0, 165.0),
                                                          metal));
    const auto left_block_offset = FreeVec3(265.0, 0.0, 295.0);
    const auto left_block_rotation = std::make_shared<RotateY>(RotateY(left_block, /*angle_in_degrees=*/29.0));
    hittable_list->add(std::make_shared<Translate>(Translate(left_block_rotation, left_block_offset)));

    return Scene{.camera=std::move(current_camera),
                 .world=std::move(hittable_list),
                 .maximum_recursion_depth=maximum_recursion_depth};
}

Scene boxes(int x_pixels, int y_pixels, int maximum_recursion_depth) {
    // Positionable camera.
    const BoundVec3 look_from(478.0, 278.0, -600.0);
    const FreeVec3 look_at(0.0, 0.0, 0.0);
    const FreeVec3 view_up(0.0, 1.0, 0.0);
    const value_type distance_to_focus = 10.0;
    const value_type aperture = 0.0;
    const value_type field_of_view = 40.0;
    const value_type time0 = 0.0;
    const value_type time1 = 1.0;
    const value_type aspect = value_type(x_pixels) / value_type(y_pixels);
    auto current_camera = std::make_unique<Camera>(Camera(look_from, look_at, view_up, field_of_view, aspect,
                                                          aperture, distance_to_focus, time0, time1));

    // World.
    const int num_hittables = 40;
    auto hittable_list = std::make_unique<HittableWorld>(HittableWorld(num_hittables));

    const auto light_texture = std::make_shared<ConstantTexture>(ConstantTexture(Color3(1.0, 1.0, 1.0)));
    const auto light = std::make_shared<DiffuseLight>(DiffuseLight(light_texture));
    const auto pertext_material = std::make_shared<Lambertian>(Lambertian(std::make_shared<NoiseTexture>(
            NoiseTexture(/*scale=*/4, /*turbulence_depth=*/7, Perlin(/*num_permutations=*/256)))));

    const auto ground = std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>
                                                    (ConstantTexture(Color3(0.48, 0.83, 0.53)))));
    int number_of_boxes = 20;
    for (int i = 0; i < number_of_boxes; ++i) {
        for (int j = 0; j < number_of_boxes; ++j) {
            const value_type w = 100.0;
            const value_type x0 = -1000.0 + i * w;
            const value_type y0 = 0.0;
            const value_type z0 = -1000.0 + j * w;
            const value_type x1 = x0 + w;
            const value_type y1 = 100 * (random_value() + 0.01);
            const value_type z1 = z0 + w;
            const auto current_block = std::make_shared<Block>(Block(BoundVec3(x0, y0, z0), BoundVec3(x1, y1, z1), ground));
            hittable_list->add(current_block);
        }
    }

    return Scene{.camera=std::move(current_camera),
            .world=std::move(hittable_list),
            .maximum_recursion_depth=maximum_recursion_depth};
}

#endif //RAYTRACING_SCENE_H
