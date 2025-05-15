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
    int image_width  = 1200;  // Rendered image width in pixel count
    int image_height = 675;
    point3 cameraLookFrom = point3(13,2,3);
    point3 cameraLookAt = point3(0,0,0);
    vec3 cameraLookUp = vec3(0,1,0);
    int fov = 60;
    int frameNum = 1;

    int samples_per_pixel = 20;

    mutable int maxDepth = 10;

    color backgroundColor = color(0.2, 0.2, 0.2);


    vec3 directionToLight = vec3(1.0, 1.0, -1.0);
    color lightColor = color(1.0, 1.0, 1.0);
    color ambientLight = color(0.1, 0.1, 0.1);


    void render(const hittable& world) {
        initialize();
        ofstream outputFile;
        string fileName = "output";
        fileName += std::to_string(frameNum);
        fileName += ".ppm";
        frameNum++;
        //std::cout << cameraLookFrom << std::endl;
        outputFile.open(fileName);


        outputFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                write_color(outputFile, pixel_samples_scale * pixel_color);
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

        directionToLight = unit_vector(directionToLight);

        float aspect_ratio = float(image_width)/float(image_height);

        float viewportHeight = 2*(cameraLookFrom.length() * tan(degrees_to_radians(fov/2)));
        float viewportWidth = aspect_ratio * viewportHeight;

        vec3 w = unit_vector(cameraLookFrom - cameraLookAt);  // Camera backward direction
        vec3 u = unit_vector(cross(cameraLookUp, w));        // Camera right direction
        vec3 v = cross(w, u);                                // Camera up direction
        
        viewport_u = viewportWidth * u;
        viewport_v = -viewportHeight * v;

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
        ray_direction = unit_vector(ray_direction);

        return ray(ray_origin, ray_direction);

    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }




    color ray_color(const ray& r, const hittable& world) const {

        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            auto rayOrigin = rec.p + rec.normal * 0.001;  // Offset to prevent self-shadowing
            auto rayDirection = directionToLight;

            ray shadowRay(rayOrigin, rayDirection);

            auto kd = rec.mat->getKD();
            auto ks = rec.mat->getKS();
            auto ka = rec.mat->getKA();
            auto od = rec.mat->getOD();
            auto os = rec.mat->getOS();
            auto kgls = rec.mat->getKgls();
            auto ref = rec.mat->getRefl();
            
            auto R = 2*rec.normal*dot(rec.normal, directionToLight) - directionToLight;
            R = R/R.length();

            if (shadow_helper(shadowRay, world, 0.01, infinity)) {
                if (ref == 0.0) {
                    return color(0.0, 0.0, 0.0);
                }
            }
            
            if (ref > 0.0) {
                auto reflectionVector = r.direction() - 2 * dot(r.direction(), rec.normal) * rec.normal;
                reflectionVector = unit_vector(reflectionVector);
                ray reflectionRay(rec.p, reflectionVector);
            
                auto krefl = reflection_helper(reflectionRay, world, maxDepth);
    
                auto reflectionContribution = krefl;
                auto V = r.origin() - rec.p;
                V = V/V.length();
                return (ka * od * ambientLight) + 
                (kd * lightColor * od * std::max(0.0, dot(rec.normal, directionToLight))) + 
                (ks * lightColor * os * pow(std::max(0.0, dot(V, R)), kgls) + reflectionContribution);
            }


        

            auto V = r.origin() - rec.p;
            V = V/V.length();
            return (ka * od * ambientLight) + 
            (kd * lightColor * od * std::max(0.0, dot(rec.normal, directionToLight))) + 
            (ks * lightColor * os * pow(std::max(0.0, dot(V, R)), kgls));
            
        }    
        return backgroundColor;
    }

    bool shadow_helper(const ray& r, const hittable& world, double min_t, double max_t) const {
        hit_record rec;
        if (world.hit(r, interval(min_t, max_t), rec)) {
            return true;  // Shadow detected
        }
        return false;  // No shadow, light reaches surface
    }    

    color reflection_helper(const ray& r, const hittable& world, int currDepth) const {
        currDepth--;
        if (currDepth <= 0) return color(0.0, 0.0, 0.0);


        hit_record new_rec;
        if (world.hit(r, interval(0.01, infinity), new_rec)) {
            auto rayOrigin = new_rec.p + new_rec.normal * 1e-6;  // Offset to prevent self-shadowing
            auto rayDirection = directionToLight;

            ray shadowRay(rayOrigin, rayDirection);

            auto kd = new_rec.mat->getKD();
            auto ks = new_rec.mat->getKS();
            auto ka = new_rec.mat->getKA();
            auto od = new_rec.mat->getOD();
            auto os = new_rec.mat->getOS();
            auto kgls = new_rec.mat->getKgls();
            auto ref = new_rec.mat->getRefl();

            auto R = 2*new_rec.normal*dot(new_rec.normal, directionToLight) - directionToLight;
            R = R/R.length();

            if (shadow_helper(shadowRay, world, 0.01, infinity)) {
                if (ref == 0.0) {
                    return color(0.0, 0.0, 0.0);
                }
            }
            
            auto V = r.origin() - new_rec.p;
            V = V/V.length();


            auto reflectionVector = r.direction() - 2 * dot(r.direction(), new_rec.normal) * new_rec.normal;
            reflectionVector = reflectionVector/reflectionVector.length();
            ray reflectionRay(new_rec.p, reflectionVector);

            color reflectionContribution = reflection_helper(reflectionRay, world, currDepth);
            
            auto value = (ka * od * ambientLight) + 
            (kd * lightColor * od * std::max(0.0, dot(new_rec.normal, directionToLight))) + 
            (ks * lightColor * os * pow(std::max(0.0, dot(V, R)), kgls));
            return value + reflectionContribution * new_rec.mat->getRefl();
        }

        
        return backgroundColor ;//* rec.mat->getRefl();
    }

};

#endif