cmake_minimum_required(VERSION 3.15)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing Hittable.h main.cpp Vec3.h Ray.h Sphere.h HittableList.h Camera.h material/Material.h material/Lambertian.h material/Metal.h util.h material/Dielectric.h)