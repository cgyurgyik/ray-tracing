# About
This repository follows the steps from [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview). Enhancements featured are provided by the blogpost about [Type-Safe Ray Tracing](https://ajeetdsouza.github.io/blog/posts/type-safe-raytracing-in-modern-cpp/).

# Features
- Demonstration using PPM image file.
- Single value_type to allow for user to switch between double, float, or a type of his or her choice.
- Abstract material class to allow for OOP approach to different materials. Current materials include lambertian, metallic, and dielectric (clear).
- Type safe vector types to mitigate bugs.
- Positionable camera.

# Examples
- Six spheres in a triangular formation.
![Example 1](images/raytracing_example1.png)
- Reflections on metallic material.
![Example 2](images/raytracing_example2.png)
- Three spheres from L to R: Dielectric, Lambertian, Metallic
![Example 3](images/raytracing_example3.png)
