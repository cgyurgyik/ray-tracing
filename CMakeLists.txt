cmake_minimum_required(VERSION 3.15)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing Hittable.h main.cpp Vec3.h Ray.h Sphere.h HittableList.h Camera.h Material.h Lambertian.h Metal.h util.h)