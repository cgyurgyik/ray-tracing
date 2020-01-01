#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H
#include <vector>
#include "Vec3.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "material/Lambertian.h"
#include "material/Metal.h"
#include "material/Dielectric.h"

// Produces a random scene with 500 spheres of varying sizes and materials. T
// Three larger spheres are then placed offset in the center.
HittableList* random_scene() {
    const int num_spheres = 500;
    Hittable** spheres = new Hittable*[num_spheres + 1];
    spheres[0] = new Sphere(BoundVec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(Color3(0.5, 0.5, 0.5)));

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
                    spheres[index++] = new Sphere(center, 0.2, new Lambertian(Color3(r, g, b)));
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
                                        new Lambertian(Color3(0.4, 0.2, 0.1)));
    spheres[index++] = new Sphere(BoundVec3(0.0, 1.0, 0.0), 1.0,
                                        new Metal(Color3(0.7, 0.6, 0.5), 0.0));
    return new HittableList(spheres, index);
}

// Creates a scene with three spheres and a background.
// From left to right: Dielectric, Lambertian, Metallic.
HittableList* three_spheres() {
    const int num_surfaces = 5;
    Hittable** list = new Hittable*[num_surfaces];

    // Background.
    list[0] = new Sphere(BoundVec3(0.0, -100.5, -1.0), 100.0,
                         new Lambertian(Color3(0.1, 0.3, 0.01)));
    // (Left) Dielectric material, hollow sphere.
    list[1] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), 0.5,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    list[2] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), -0.48,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    // (Middle) Lambertian material.
    list[3] = new Sphere(BoundVec3(0.0, 0.0, -1.0), 0.5,
                         new Lambertian(Color3(0.1, 0.2, 0.5)));
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
                         new Lambertian(Color3(0.1, 0.3, 0.01)));

    // Centered metallic material.
    const BoundVec3 center(0.0, 0.0, -1.0);
    list[1] = new Sphere(center, 0.5,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));

    list[2] = new Sphere(BoundVec3(-0.5, 0.0, 0.0), 0.1,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    list[3] = new Sphere(BoundVec3(0.4, 0.0, 0.0), 0.11,
                         new Lambertian(Color3(0.3, 0.2, 0.1)));
    list[4] = new Sphere(BoundVec3(0.7, 0.0, 0.0), 0.1,
                         new Lambertian(Color3(0.5, 0.2, 0.4)));
    list[5] = new Sphere(BoundVec3(1.0, 0.0, 0.0), 0.13,
                         new Lambertian(Color3(0.1, 0.5, 0.2)));
    list[6] = new Sphere(BoundVec3(0.0, 1.0, 0.0), 0.1,
                         new Metal(Color3(0.5, 0.5, 0.5), /*fuzz=*/0.01));
    list[7] = new Sphere(BoundVec3(0.0, 0.5, 0.5), 0.1,
                         new Metal(Color3(0.2, 0.2, 0.2), /*fuzz=*/0.01));
    list[8] = new Sphere(BoundVec3(-0.5, 0.0, -0.5), 0.101,
                         new Lambertian(Color3(0.8, 0.6, 0.2)));
    list[9] = new Sphere(BoundVec3(0.9, 0.0, -0.5), 0.11,
                         new Lambertian(Color3(0.8, 0.8, 0.2)));
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
                         new Lambertian(Color3(0.1, 0.3, 0.01)));
    // (Back Left) Dielectric material.
    list[1] = new Sphere(BoundVec3(-1.2, 0.0, -1.0), 0.5,
                         new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    // (Back Middle) Lambertian material.
    list[2] = new Sphere(BoundVec3(0.0, 0.0, -1.0), 0.5,
                         new Lambertian(Color3(0.1, 0.2, 0.5)));
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

#endif //RAYTRACING_SCENE_H
