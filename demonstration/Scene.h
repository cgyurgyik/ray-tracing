#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include <vector>
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

// Produces a random scene with 500 spheres of varying sizes and materials. T
// Three larger spheres are then placed offset in the center.
HittableList* random_scene() {
    const int num_spheres = 500;
    Hittable** spheres = new Hittable*[num_spheres + 1];
    spheres[0] = new Sphere(BoundVec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(new ConstantTexture(Color3(0.5, 0.5, 0.5))));

    int index = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            const value_type choose_material = random_value();
            BoundVec3 center(a + 0.9 * random_value(), 0.2, b + 0.9 * random_value());
            if ((center - FreeVec3(4.0, 0.2, 0.0)).length() > 0.9) {
                value_type r;
                value_type g;
                value_type b;
                if (choose_material < 0.8) { // Diffuse
                    r = random_value() * random_value();
                    g = random_value() * random_value();
                    b = random_value() * random_value();
                    spheres[index++] = new Sphere(center, 0.2, new Lambertian(new ConstantTexture(Color3(r, g, b))));
                } else if (choose_material < 0.95) { // Metal
                    r = 0.5 * (1 + random_value());
                    g = 0.5 * (1 + random_value());
                    b = 0.5 * (1 + random_value());
                    spheres[index++] = new Sphere(center, 0.2, new Metal(Color3(r, g, b), 0.5 * random_value()));
                } else { // Glass
                    spheres[index++] = new Sphere(center, 0.2,
                            new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
                }
            }
        }
    }
    spheres[index++] = new Sphere(BoundVec3(0.0, 1.0, 0.0), 1.0,
            new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    spheres[index++] = new Sphere(BoundVec3(-4.0, 1.0, 0.0), 1.0,
                                        new Lambertian(new ConstantTexture(Color3(0.4, 0.2, 0.1))));
    spheres[index++] = new Sphere(BoundVec3(0.0, 1.0, 0.0), 1.0,
                                        new Metal(Color3(0.7, 0.6, 0.5), 0.0));
    return new HittableList(spheres, index);
}

// Creates a scene with two opposing checkered spheres.
//
// Positionable camera:
// vec3 look_from(13,2,3);
// vec3 look_at(0,0,0);
// float dist_to_focus = 10.0;
// float aperture = 0.0;
// time0: 0.0, time1: 1.0
HittableList* opposing_checkered_spheres() {
    ConstantTexture* texture1 = new ConstantTexture(Color3(0.2, 0.3, 0.1));
    ConstantTexture* texture2 = new ConstantTexture(Color3(0.9, 0.9, 0.9));
    Texture* checker1 = new CheckerTexture(texture1, texture2);

    ConstantTexture* texture11 = new ConstantTexture(Color3(0.7, 0.5, 0.2));
    ConstantTexture* texture22 = new ConstantTexture(Color3(0.5, 0.5, 0.5));
    Texture* checker2 = new CheckerTexture(texture11, texture22);

    const int num_surfaces = 2;
    Hittable **list = new Hittable*[num_surfaces];
    list[0] = new Sphere(BoundVec3(0.0, -10.0, 0.0), 10.0, new Lambertian(checker1));
    list[1] = new Sphere(BoundVec3(0.0, 10.0, 0.0), 10.0, new Lambertian(checker2));
    return new HittableList(list, num_surfaces);
}

// Creates a scene with three spheres and a background.
// From left to right: Dielectric, Lambertian, Metallic.
HittableList* three_spheres() {
    const int num_surfaces = 5;
    Hittable** list = new Hittable*[num_surfaces];

    // Background.
    list[0] = new Sphere(BoundVec3(0.0, -100.5, -1.0), 100.0,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.3, 0.01))));
    // (Left) Dielectric material, hollow sphere.
    list[1] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), 0.5,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    list[2] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), -0.48,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    // (Middle) Lambertian material.
    list[3] = new Sphere(BoundVec3(0.0, 0.0, -1.0), 0.5,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.2, 0.5))));
    // (Right) Metallic material.
    list[4] = new Sphere(BoundVec3(1.2, 0.0, -1.0), 0.5,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));

    return new HittableList(list, num_surfaces);
}

// Places a single large metallic spheres with a number of smaller
// lambertian and dielectric in front of it.
HittableList* reflections() {
    const int num_surfaces = 10;
    Hittable** list = new Hittable*[num_surfaces];

    // Background.
    list[0] = new Sphere(BoundVec3(0.0, -100.5, -1.0), 100.0,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.3, 0.01))));

    // Centered metallic material.
    const BoundVec3 center(0.0, 0.0, -1.0);
    list[1] = new Sphere(center, 0.5,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));

    list[2] = new Sphere(BoundVec3(-0.5, 0.0, 0.0), 0.1,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    list[3] = new Sphere(BoundVec3(0.4, 0.0, 0.0), 0.11,
                         new Lambertian(new ConstantTexture(Color3(0.3, 0.2, 0.1))));
    list[4] = new Sphere(BoundVec3(0.7, 0.0, 0.0), 0.1,
                         new Lambertian(new ConstantTexture(Color3(0.5, 0.2, 0.4))));
    list[5] = new Sphere(BoundVec3(1.0, 0.0, 0.0), 0.13,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.5, 0.2))));
    list[6] = new Sphere(BoundVec3(0.0, 1.0, 0.0), 0.1,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));
    list[7] = new Sphere(BoundVec3(0.0, 0.5, 0.5), 0.1,
                         new Metal(Color3(0.2, 0.2, 0.2), /*fuzz=*/0.01));
    list[8] = new Sphere(BoundVec3(-0.5, 0.0, -0.5), 0.101,
                         new Lambertian(new ConstantTexture(Color3(0.8, 0.6, 0.2))));
    list[9] = new Sphere(BoundVec3(0.9, 0.0, -0.5), 0.11,
                         new Lambertian(new ConstantTexture(Color3(0.8, 0.8, 0.2))));
    return new HittableList(list, num_surfaces);
}

// Places 6 spheres in a triangular formation:
// X X X
//  X X
//   X
HittableList* triangular_formation_spheres() {
    const int num_surfaces = 7;
    Hittable **list = new Hittable *[num_surfaces];

    // Background.
    list[0] = new Sphere(BoundVec3(0.0, -100.5, -1.0), 100.0,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.3, 0.01))));
    // (Back Left) Dielectric material.
    list[1] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), 0.5,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    // (Back Middle) Lambertian material.
    list[2] = new Sphere(BoundVec3(0.0, 0.0, -1.0), 0.5,
                         new Lambertian(new ConstantTexture(Color3(0.1, 0.2, 0.5))));
    // (Back Right) Metallic material.
    list[3] = new Sphere(BoundVec3(1.2, 0.0, -1.0), 0.5,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));
    // (Middle Left)
    list[4] = new Sphere(BoundVec3(-0.4, 0.0, 0.0), 0.2,
                         new Metal(Color3(0.3, 0.5, 0.6), /*fuzz=*/0.0));
    // (Middle Right)
    list[5] = new Sphere(BoundVec3(0.4, 0.0, 0.0), 0.2,
                         new Metal(Color3(0.6, 0.5, 0.4), /*fuzz=*/0.01));
    // (Front)
    list[6] = new Sphere(BoundVec3(0.0, 0.0, 0.8), 0.4,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));

    return new HittableList(list, num_surfaces);
}

// Creates a simple rectangular light fixture against a sphere.
HittableList* simple_light_fixture() {
    ConstantTexture* texture1 = new ConstantTexture(Color3(0.3, 0.3, 0.3));
    ConstantTexture* texture2 = new ConstantTexture(Color3(0.9, 0.9, 0.9));
    Texture* checker = new CheckerTexture(texture1, texture2);
    const int num_surfaces = 4;
    Hittable **list = new Hittable*[num_surfaces];
    list[0] = new Sphere(BoundVec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(new ConstantTexture(Color3(0.1, 0.1, 0.1))));
    list[1] = new Sphere(BoundVec3(0.0, 2.0, 0.0), 2.0, new Lambertian(checker));
    list[2] = new Sphere(BoundVec3(0.0, 7.0, 0.0), 2.0, new DiffuseLight(new ConstantTexture(Color3(4.0, 4.0, 4.0))));
    list[3] = new Rectangle_XY(/*x0=*/3.0, /*x1=*/5.0, /*y0=*/1.0, /*y1=*/3.0, /*k=*/-2.0,
                                      new DiffuseLight(new ConstantTexture(Color3(4.0, 4.0, 4.0))));
    return new HittableList(list, num_surfaces);
}

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
HittableList* cornell_box() {
    const int num_surfaces = 8;
    Hittable** list = new Hittable*[num_surfaces];

    Material *red_material = new Lambertian(new ConstantTexture(Color3(0.65, 0.05, 0.05)));
    Material *white_material = new Lambertian(new ConstantTexture(Color3(0.73, 0.73, 0.73)));
    Material *green_material = new Lambertian(new ConstantTexture(Color3(0.12, 0.45, 0.15)));
    Material *light = new DiffuseLight(new ConstantTexture(Color3(7.0, 7.0, 7.0)));

    // Left wall.
    list[0] = new FlipNormals(new Rectangle_YZ(0, 555, 0, 555, 555, green_material));

    // Right wall.
    list[1] = new Rectangle_YZ(0, 555, 0, 555, 0, red_material);

    // Light source.
    list[2] = new Rectangle_XZ(113, 443, 127, 432, 554, light);

    list[3] = new FlipNormals(new Rectangle_XZ(0, 555, 0, 555, 555, white_material));
    list[4] = new Rectangle_XZ(0, 555, 0, 555, 0, white_material);
    list[5] = new FlipNormals(new Rectangle_XY(0, 555, 0, 555, 555, white_material));

    // Left block.
    const auto left_block = new Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 165.0, 165.0), white_material);
    const auto left_block_offset = FreeVec3(130.0, 0.0, 65.0);
    const value_type left_block_rotation = -18.0;
    list[6] = new Translate(new RotateY(left_block, left_block_rotation), left_block_offset);

    // Right block.
    const auto right_block = new Block(BoundVec3(0.0, 0.0, 0.0), BoundVec3(165.0, 330.0, 165.0), white_material);
    const auto right_block_offset = FreeVec3(265.0, 0.0, 295.0);
    const value_type right_block_rotation = 15.0;
    list[7] = new Translate(new RotateY(right_block, right_block_rotation), right_block_offset);

    return new HittableList(list, num_surfaces);
}

#endif //RAYTRACING_SCENE_H
