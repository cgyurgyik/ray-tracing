#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Vec3.h"
#include "raytracing_utility_functions.h"

// Creates a P3 PPM File. Colors are represented in ASCII.
// 'portrait' represents the corresponding layout using 3-dimensional color vectors.
// 'max_color' represents the maximum color value, [0, max_color].
void CreatePPMFile(std::vector<std::vector<Color3>> portrait, int max_color) {
    std::ofstream file;
    file.open("raytracing_demo.ppm");
    if (!file) {
        throw std::runtime_error("\nError opening the file.");
    }
    const auto num_rows = portrait.size();
    const auto num_columns = portrait[0].size();

    file << "P3\n" ;
    file << num_columns << " " << num_rows;
    file << "\n" << max_color << "\n";

    // Bottom to top, left to right.
    for (int row = num_rows - 1; row >= 0; --row) {
        for (int column = 0; column < num_columns; ++column) {
            const int i_red = portrait[row][column].r();
            const int i_green = portrait[row][column].g();
            const int i_blue = portrait[row][column].b();
            file << i_red << " " << i_green << " " << i_blue << "\n";
        }
    }
    file.close();
}

int main() {
    const int num_rows = 200;
    const int num_columns = 100;
    const int max_color = 255;

    BoundVec3 lower_left_corner(-2.0, -1.0, -1.0);
    FreeVec3 horizontal(4.0, 0.0, 0.0);
    FreeVec3 vertical(0.0, 2.0, 0.0);
    BoundVec3 origin(0.0, 0.0, 0.0);

    std::vector<std::vector<Color3>> ppm_portrait(num_columns, std::vector<Color3>(num_rows, Color3(0.0, 0.0, 0.0)));
    for (int current_column = num_columns - 1; current_column >= 0; --current_column) {
        for (int current_row = 0; current_row < num_rows; ++current_row) {
            const value_type u = value_type(current_row) / value_type(num_rows);
            const value_type v = value_type(current_column) / value_type(num_columns);
            Ray ray(origin, UnitVec3(lower_left_corner + horizontal * u + vertical * v));
            const Color3 col = color(ray);
            ppm_portrait[current_column][current_row].r() = int(255.99 * col.r());
            ppm_portrait[current_column][current_row].g() = int(255.99 * col.g());
            ppm_portrait[current_column][current_row].b() = int(255.99 * col.b());
        }
    }
    CreatePPMFile(ppm_portrait, max_color);
}
