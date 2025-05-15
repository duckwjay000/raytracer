#include <fstream>

#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"

#include "camera.h"
#include <random>

using namespace std;

int main() {
    hittable_list world;



    //white
    auto material_white = make_shared<material>(0.8, 0.1, 0.3, color(1.0, 1.0, 1.0), color(1.0, 1.0, 1.0), 4.0);
    //purple
    auto material_purple = make_shared<material>(0.7, 0.2, 0.1, color(1.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 16.0);
    //red
    auto material_red = make_shared<material>(0.8, 0.8, 0.1, color(1.0, 0.0, 0.0), color(0.5, 1.0, 0.5), 32.0);
    //green
    auto material_green = make_shared<material>(0.7, 0.5, 0.1, color(0.0, 1.0, 0.0), color(0.5, 1.0, 0.5), 64.0);
    //blue
    auto material_blue = make_shared<material>(0.9, 1.0, 0.1, color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 32.0);
    // yellow
    auto material_yellow = make_shared<material>(0.9, 1.0, 0.1, color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 4.0);
    // black
    auto material_black = make_shared<material>(0.8, 0.1, 0.3, color(0.0, 0.0, 0.0), color(1.0, 1.0, 1.0), 16.0);
    //gray
    auto material_gray = make_shared<material>(0.8, 0.1, 0.3, color(0.7, 0.7, 0.7), color(1.0, 1.0, 1.0), 16.0);


    //reflective
    auto material_reflective = make_shared<material>(0.0, 0.1, 0.1, color(0.75, 0.75, 0.75), color(1.0, 1.0, 1.0), 10.0, 0.9);


    
    //make_shared<triangle>(point3)
    
    
    //world.add(make_shared<sphere>(point3(0.45, 0.0, -0.15), 0.15, material_white));
    //red
    //world.add(make_shared<sphere>(point3(0.0, 0.0, -0.1), 0.2, material_red));
    //green
    //world.add(make_shared<sphere>(point3(-0.6, 0.0, 0.0), 0.3, material_green));
    //blue
    //world.add(make_shared<sphere>(point3(0.0, 0-10000.5, 0.0), 10000.0, material_blue));

    

    //world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_purple));
    //world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_purple));


    //img 1
    //world.add(make_shared<sphere>(point3(0.0, 0.3, -1.0), 0.25, material_reflective));
    //world.add(make_shared<triangle>(point3(0.0, -0.7, -0.5), point3(1.0, 0.4, -1.0), point3(0.0, -0.7, -1.5), material_blue));
    //world.add(make_shared<triangle>(point3(0.0, -0.7, -0.5), point3(0.0, -0.7, -1.5), point3(-1.0, 0.4, -1.0), material_yellow));


    //img 2
    //world.add(make_shared<sphere>(point3(0.5, 0.0, -0.15), 0.05, material_white));
    //world.add(make_shared<sphere>(point3(0.3, 0.0, -0.1), 0.08, material_red));
    //world.add(make_shared<sphere>(point3(-0.6, 0.0, 0.0), 0.3, material_green));
    //reflective sphere
    //world.add(make_shared<sphere>(point3(0.1, -0.55, 0.25), 0.3, material_reflective));

    //world.add(make_shared<triangle>(point3(0.3, -0.3, -0.4), point3(0.0, 0.3, -0.1), point3(-0.3, -0.3, 0.2), material_blue));
    //world.add(make_shared<triangle>(point3(-0.2, 0.1, 0.1), point3(-0.2, -0.5, 0.2), point3(-0.2, 0.1, -0.3), material_yellow));

    std::random_device rd; // Seed for the generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, 7); // Distribution between 0 and 7

    // Generate a random number






    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material_gray));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    auto material_rand = make_shared<material>(0.8, 0.1, 0.3, albedo, color(1.0, 1.0, 1.0), 16.0);

                    world.add(make_shared<sphere>(center, 0.2, material_rand));
                } else  {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    world.add(make_shared<sphere>(center, 0.2, material_reflective));
                }
            }
        }
    }


    auto material_transmissive = make_shared<material>(0.0, 0.1, 0.1, color(0.75, 0.75, 0.75), color(1.0, 1.0, 1.0), 10.0, 0.9, 1.5);

    world.add(make_shared<sphere>(point3(-.1,1,0), 1.0, material_transmissive));
    world.add(make_shared<sphere>(point3(-4.1,1,0), 1.0, material_red));
    world.add(make_shared<sphere>(point3(4.1,1,0), 1.0, material_blue));





    camera cam;


    point3 camLookFrom = point3(13,2,3);


    for (int i = 0; i < 359; i++) {
        cam.cameraLookFrom = camLookFrom;
        
        camLookFrom = camLookFrom.rotate_y(M_PI/180);
        cam.render(world);
        std::cout << "done with " << i << std::endl;
    }

    cam.render(world);

}