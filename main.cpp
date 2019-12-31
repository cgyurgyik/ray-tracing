#include <iostream>
#include <fstream>
#include <vector>
#include "Vec3.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "raytracing_utility_functions.h"

// Creates a P3 PPM File. Colors are represented in ASCII.
int main() {
    // Dimensions for the PPM image.
    const int x_pixels = 200;
    const int y_pixels = 100;
    const int num_runs = 100; // The average number of runs, for antialiasing.


    // 'max_color' represents the maximum color value.
    const int max_color = 255;

    // Simple axis-aligned camera.
    const BoundVec3 lower_left_corner(-2.0, -1.0, -1.0);
    const FreeVec3 horizontal(4.0, 0.0, 0.0);
    const FreeVec3 vertical(0.0, 2.0, 0.0);
    const BoundVec3 origin(0.0, 0.0, 0.0);
    const Camera camera(origin, lower_left_corner, horizontal, vertical);

    const int num_surfaces = 2;
    Hittable *list[num_surfaces];
    list[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5);
    list[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0);
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

    // Top to bottom, left to right.
    for (int j = y_pixels - 1; j >= 0; --j) {
        for (int i = 0; i < x_pixels; ++i) {
            Color3 current_color(0.0, 0.0, 0.0);
            antialiasing(current_color, camera, world,
                    num_runs, x_pixels, y_pixels, i, j);
            current_color = dampen(current_color);
            const value_type color_value = max_color + value_type(0.99);
            const int i_red = int(color_value * current_color.r());
            const int i_green = int(color_value * current_color.g());
            const int i_blue = int(color_value * current_color.b());
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}
