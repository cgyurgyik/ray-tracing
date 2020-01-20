#include <iostream>
#include <fstream>
#include "../utility/Vec3.h"
#include "../surfaces/HittableWorld.h"
#include "../utility/Camera.h"
#include "Scene.h"

// A demonstration that generates a PPM file named "raytracing_demo.ppm"
// using the current Scene.
int main() {
    const int x_pixels = 200;
    const int y_pixels = 200;

    // The average number of samples, for antialiasing.
    const int num_samples = 50;

    // 'max_color' represents the maximum color value.
    const int max_color = 255;

    // The maximum recursion depth allowed for coloring.
    const int maximum_depth = 50;

    // Scene.
    const Scene scene = perlin_noise_demonstration(x_pixels, y_pixels, maximum_depth);

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
                    num_samples, x_pixels, y_pixels, i, j, scene.maximum_recursion_depth);
            Camera::dampen(current_color);

            const int i_red = int(max_color * current_color.r());
            const int i_green = int(max_color * current_color.g());
            const int i_blue = int(max_color * current_color.b());
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}