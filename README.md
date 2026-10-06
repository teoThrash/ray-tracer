# Ray Tracer (C++)

A CPU ray tracer written in C++, built by following Peter Shirley's [*Ray Tracing in One Weekend*](https://raytracing.github.io/) book series, which I extended with [describe your own additions, or delete this clause].

![Render](docs/demo.png)

## Features

- Vector math and ray/camera model with a configurable camera (field of view, position, depth of field)
- Ray-sphere and ray-quad intersection
- Materials: diffuse (Lambertian), metal, and dielectric (glass), plus [emissive lights, if included]
- Bounding Volume Hierarchy (BVH) with axis-aligned bounding boxes to speed up ray-object intersection
- Textures: solid colour, image textures (Earth map, loaded with `stb_image`) and Perlin noise
- Anti-aliasing through multiple samples per pixel, with gamma correction
- [Your own additions, e.g. multithreading, new scenes, a new shape, performance changes]

## How it works

For each pixel, the camera fires several rays into the scene. Each ray is tested against the objects, and the BVH skips whole groups of objects whose bounding box the ray misses. This reduces the cost of an intersection test from linear to roughly logarithmic in the number of objects. When a ray hits a surface, the material decides whether it scatters, reflects or refracts, and the ray is followed recursively up to a maximum depth. The resulting colours are averaged and written to a PPM image.

## Build and run

Requirements: a C++17 compiler and [CMake](https://cmake.org/).

```
cmake -B build
cmake --build build --config Release
```

Then run the executable and save the output to an image:

```
[path to the executable] > image.ppm
```

PPM files can be opened with GIMP, IrfanView or similar, or converted to PNG with ImageMagick.

## Project structure

| File | Purpose |
|------|---------|
| `vec3.h`, `ray.h`, `interval.h`, `color.h` | Math and utility types |
| `camera.h` | Camera and rendering loop |
| `hittable.h`, `hittable_list.h`, `sphere.h`, `quad.h` | Scene objects and intersection |
| `aabb.h`, `bvh.h` | Bounding boxes and acceleration structure |
| `material.h` | Diffuse, metal and dielectric materials |
| `texture.h`, `perlin.h`, `rtw_stb_image.h` | Textures, Perlin noise and image loading |

## Credits

Based on *Ray Tracing in One Weekend* and *Ray Tracing: The Next Week* by Peter Shirley, Trevor David Black and Steve Hollasch. Image loading uses [`stb_image`](https://github.com/nothings/stb) (public domain).