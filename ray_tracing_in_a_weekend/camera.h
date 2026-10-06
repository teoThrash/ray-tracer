#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

using namespace std;

class camera{
public:

    double aspect_ratio = 1.0;
    int image_width = 100;
    int max_depth = 10;
    int samples_per_pixel = 10;
    double vfov = 90; //vertical angle
    color background;

    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

        
    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable& world){
        initialize();

        cout<<"P3\n"<<image_width << " " <<image_height<<"\n255\n";

        for(int j=0; j<image_height; j++){
            clog<<"\r Scanlines remaining: "<<(image_height-j)<<' '<<flush;
            for(int i=0; i<image_width; i++){
                //sampling

                color pixel_color(0, 0, 0);

                for(int s=0; s<samples_per_pixel; s++){
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(cout, pixel_samples_scale * pixel_color);
            }
        }

        clog<<"\rDone.             \n";
    }

private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u, pixel_delta_v;
    vec3 v, u, w;

    vec3 defocus_disk_u, defocus_disk_v;

    double pixel_samples_scale;

    void initialize(){
        //image dimensions
        
        image_height = int(image_width/aspect_ratio);
        image_height = max(image_height, 1);

        center = lookfrom;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        //viewport dimensions

        //auto focal_length = (lookfrom - lookat).length();

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);

        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        //base

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        //horizontal and vertical vectors

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        //pixel deltas

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

        //defocus disk basis vectors
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    ray get_ray(int i, int j){
        //gives me a camera ray from the origin and going somewhere randomly sampled around i, j

        auto offset = sample_square();

        auto pixel_sample = pixel00_loc + ((i+offset.x())*pixel_delta_u) + ((j+offset.y())*pixel_delta_v);

        //make the new ray
        auto ray_origin = (defocus_angle <=0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        auto ray_time=random_double();


        return ray(ray_origin, ray_direction, ray_time);

    }

    vec3 sample_square(){
        //returns a vector to a random point in the [-.5, -.5] - [+.5, +.5] square

        return vec3(random_double()-0.5, random_double()-0.5, 0);
    }

    point3 defocus_disk_sample() const {
        //random point in the defocus disk

        auto p = random_in_unit_disk();
        return center + (p[0]*defocus_disk_u) + (p[1]*defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        //stop recursion
        if(depth<=0) return color(0, 0, 0);
        
        hit_record rec;

        if(!world.hit(r, interval(0.001, infinity), rec)) return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if(!rec.mat->scatter(r, rec, attenuation, scattered)) return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif