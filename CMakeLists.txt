cmake_minimum_required(VERSION 3.15)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing surfaces/Hittable.h demonstration/main.cpp utility/Vec3.h utility/Ray.h surfaces/Sphere.h surfaces/HittableList.h utility/Camera.h material/Material.h material/Lambertian.h material/Metal.h utility/util.h material/Dielectric.h demonstration/Scene.h)