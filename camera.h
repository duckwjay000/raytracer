#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include <fstream>
#include "color.h"
#include "utility.h"
#include "material.h"
using namespace std;

class camera {
  public:
    int image_width  = 512;  // Rendered image width in pixel count
    int image_height = 512;
    point3 cameraLookFrom = point3(0,0,1);
    point3 cameraLookAt = point3(0,0,0);
    vec3 cameraLookUp = vec3(0,1,0);
    int fov = 100;

    int samples_per_pixel = 10;

    void render(const hittable& world) {
        initialize();
        ofstream outputFile;
        outputFile.open("output.ppm");


        outputFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                write_color(outputFile, pixel_samples_scale * pixel_color);



                //auto pixel_center = pixel00_loc + (i*pixel_delta_u) + (j*pixel_delta_v);
                //auto ray_direction = pixel_center - cameraLookFrom;
                //ray r(cameraLookFrom, ray_direction);
                //color pixel_color = ray_color(r, world);
                //write_color(outputFile, pixel_color);

            }
        }
    }

  private:
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    float viewportWidth;
    float viewportHeight;
    vec3 viewport_u;
    vec3 viewport_v;
    point3 viewport_upperLeft;

    double pixel_samples_scale;



    void initialize() {

        float viewportWidth = 2*(cameraLookFrom.length() * tan(degrees_to_radians(fov/2)));
        float viewportHeight = 2*(cameraLookFrom.length() * tan(degrees_to_radians(fov/2)));

        viewport_u = vec3(viewportWidth, 0, 0);
        viewport_v = vec3(0, -viewportHeight, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        viewport_upperLeft = cameraLookAt - cameraLookFrom - viewport_u/2 -viewport_v/2;
        pixel00_loc = viewport_upperLeft + 0.5 * (pixel_delta_u + pixel_delta_v);

        pixel_samples_scale = 1.0 / samples_per_pixel;
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);
        auto ray_origin = cameraLookFrom;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);

    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }




    color ray_color(const ray& r, const hittable& world) const {
        auto directionToLight = vec3(0.0, 1.0, 0.0);
        directionToLight = directionToLight/directionToLight.length();
        auto lightColor = color(1.0, 1.0, 1.0);
        auto ambientLight = color(0.1, 0.1, 0.1);

        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            //need to check shadow ray here
            auto rayOrigin = rec.p + rec.normal * 0.001;  // Offset to prevent self-shadowing
            auto rayDirection = directionToLight;

            ray shadowRay(rayOrigin, rayDirection);

            double tl = infinity;  // For directional lights, we assume infinite distance

            auto kd = rec.mat->getKD();
            auto ks = rec.mat->getKS();
            auto ka = rec.mat->getKA();
            auto od = rec.mat->getOD();
            auto os = rec.mat->getOS();
            auto kgls = rec.mat->getKgls();
            double si = 1.0;

            if (shadow_helper(shadowRay, world, 0.01, tl)) {
                si = 0;
            }
            
            //si
            //ipi

            auto R = 2*rec.normal*dot(rec.normal, directionToLight) - directionToLight;
            auto V = r.origin() - rec.p;
            V = V/V.length();
            return (ka * od * ambientLight) + 
            (si * kd * lightColor * od * std::max(0.0, dot(rec.normal, directionToLight))) + 
            (si * ks * lightColor * os * pow(std::max(0.0, dot(V, R)), kgls));
     
        }    
        return color(0.2, 0.2, 0.2);
    }

    bool shadow_helper(const ray& r, const hittable& world, double min_t, double max_t) const {
        hit_record rec;
        if (world.hit(r, interval(min_t, max_t), rec)) {

            return true;  // Shadow detected
        }
        return false;  // No shadow, light reaches surface
    }    

};

#endif