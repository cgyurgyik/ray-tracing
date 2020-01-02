#include <iostream>
#include <fstream>
#include <vector>
#include "../utility/Vec3.h"
#include "../surfaces/HittableList.h"
#include "../utility/Camera.h"
#include "Scene.h"

// Demonstration that creates a P3 PPM File.
// Note that colors are represented in ASCII.
int main() {
    // Dimensions for the PPM image.
    const int x_pixels = 500;
    const int y_pixels = 200;

    // The average number of runs, for antialiasing.
    const int num_runs = 100;


    // 'max_color' represents the maximum color value.
    const int max_color = 255;

    // Positionable camera.
    const BoundVec3 look_from(13.3, 2.0, 3.0);
    const FreeVec3 look_at(0.0, 0.0, 0.0);
    const FreeVec3 view_up(0.0, 1.0, 0.0);
    const value_type distance_to_focus = 10.0;
    const value_type aperture = 0.0;
    const value_type field_of_view = 20;
    const value_type aspect = value_type(x_pixels)/value_type(y_pixels);
    const Camera camera(look_from, look_at, view_up, field_of_view, aspect, aperture, distance_to_focus);

    // World.
    HittableList *world = simple_light_fixture();

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

            Camera::antialiasing(current_color, camera, world,
                    num_runs, x_pixels, y_pixels, i, j, depth);
            Camera::dampen(current_color);

            const int i_red = int(max_color * current_color.r());
            const int i_green = int(max_color * current_color.g());
            const int i_blue = int(max_color * current_color.b());
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}