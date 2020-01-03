cmake_minimum_required(VERSION 3.15)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing surfaces/Hittable.h demonstration/main.cpp utility/Vec3.h utility/Ray.h surfaces/Sphere.h surfaces/HittableCollection.h utility/Camera.h material/Material.h material/Lambertian.h material/Metal.h utility/util.h material/Dielectric.h demonstration/Scene.h material/DiffuseLight.h material/Texture.h material/ConstantTexture.h material/CheckerTexture.h surfaces/Rectangle_XY.h surfaces/AxisAlignedBoundingBox.h surfaces/Rectangle_XZ.h surfaces/Rectangle_YZ.h surfaces/FlipNormals.h surfaces/Block.h surfaces/Translate.h surfaces/RotateY.h)