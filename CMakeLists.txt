cmake_minimum_required(VERSION 3.15)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing main.cpp Vec3.h raytracing_utility_functions.h Hittable.h Sphere.h HittableList.h)