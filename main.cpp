#include <iostream>
#include <fstream>
#include <vector>
#include "Vec3.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "util.h"
#include "material/Lambertian.h"
#include "material/Metal.h"
#include "material/Dielectric.h"

// Demonstration that creates a P3 PPM File.
// Note: Colors are represented in ASCII.
int main() {
    // Dimensions for the PPM image.
    const int x_pixels = 500;
    const int y_pixels = 200;

    // The average number of runs, for antialiasing.
    const int num_runs = 100;


    // 'max_color' represents the maximum color value.
    const int max_color = 255;

    // Simple axis-aligned camera.
    const BoundVec3 lower_left_corner(-2.0, -1.0, -1.0);
    const FreeVec3 horizontal(4.0, 0.0, 0.0);
    const FreeVec3 vertical(0.0, 2.0, 0.0);
    const BoundVec3 origin(0.0, 0.0, 0.0);
    const Camera camera(origin, lower_left_corner, horizontal, vertical);

    // World.
    const int num_surfaces = 5;
    Hittable *list[num_surfaces];
    list[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5, new Lambertian(Color3(0.1, 0.2, 0.5)));
    list[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0, new Lambertian(Color3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Vec3(1.0, 0.0, -1.0), 0.5, new Metal(Color3(0.8, 0.6, 0.2), /*fuzz=*/0.3));
    list[3] = new Sphere(Vec3(-1.0, 0.0, -1.0), 0.5, new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    list[4] = new Sphere(Vec3(-1.0, 0.0, -1.0), -0.45, new Dielectric(DIELECTRIC_MATERIAL_REFRACTIVE_INDEX::GLASS_MID));
    HittableList *world = new HittableList(list, num_surfaces);

    // Print to the file.
    std::ofstream file;
    file.open("raytracing_demo.ppm");
    if (!file) {
        throw std::runtime_error("\nError opening the file.");
    }

    // PPM metadata.
    file << "P3\n" ;
    file << x_pixels << " " << y_pixels;
    file << "\n" << max_color << "\n";

    const int depth = 0;

    // Top to bottom, left to right.
    for (int j = y_pixels - 1; j >= 0; --j) {
        for (int i = 0; i < x_pixels; ++i) {
            Color3 current_color;

            antialiasing(current_color, camera, world,
                    num_runs, x_pixels, y_pixels, i, j, depth);
            current_color = dampen(current_color);

            const int i_red = int(max_color * current_color.r());
            const int i_green = int(max_color * current_color.g());
            const int i_blue = int(max_color * current_color.b());
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}
