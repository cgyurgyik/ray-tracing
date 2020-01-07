#include <iostream>
#include <fstream>
#include <vector>
#include "../utility/Vec3.h"
#include "../surfaces/HittableWorld.h"
#include "../utility/Camera.h"
#include "Scene.h"

// The necessary metadata required to generate P3 PPM file.
// Note that colors are represented in ASCII.
// struct P3_PPM_Image_Metadata {
//    // Dimensions for the PPM image.
//    int x_pixels, y_pixels;
//    // The number of runs for antialiasing.
//    int number_of_runs;
//    // The maximum color value.
//    int max_color;
// };

// A demonstration that generates a PPM file named "raytracing_demo.ppm"
// using the current Scene.
int main() {
    const int x_pixels = 500;
    const int y_pixels = 500;

    // The average number of runs, for antialiasing.
    const int num_runs = 200;

    // 'max_color' represents the maximum color value.
    const int max_color = 255;

    // The maximum depth allowed for coloring.
    const int maximum_depth = 30;

    // Scene.
    const Scene scene = testing_box(x_pixels, y_pixels, maximum_depth);

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
            Color3 current_color;

            Camera::antialiasing(current_color, scene.camera.get(), scene.world.get(),
                    num_runs, x_pixels, y_pixels, i, j, scene.maximum_recursion_depth);
            Camera::dampen(current_color);

            const int i_red = int(max_color * current_color.r());
            const int i_green = int(max_color * current_color.g());
            const int i_blue = int(max_color * current_color.b());
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}