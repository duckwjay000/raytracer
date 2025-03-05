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
    auto material_red = make_shared<material>(0.6, 0.3, 0.1, color(1.0, 0.0, 0.0), color(1.0, 1.0, 1.0), 32.0);
    //green
    auto material_green = make_shared<material>(0.7, 0.2, 0.1, color(0.0, 1.0, 0.0), color(0.5, 1.0, 0.5), 64.0);
    //blue
    auto material_blue = make_shared<material>(0.9, 1.0, 0.1, color(0.0, 0.0, 1.0), color(1.0, 1.0, 1.0), 4.0);
    // yellow
    auto material_yellow = make_shared<material>(0.9, 1.0, 0.1, color(1.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 4.0);
    // black
    auto material_black = make_shared<material>(0.8, 0.1, 0.3, color(0.0, 0.0, 0.0), color(1.0, 1.0, 1.0), 16.0);

    
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



    world.add(make_shared<sphere>(point3(0.0, 0.3, -1.0), 0.25, material_purple));

    


    world.add(make_shared<triangle>(point3(0.0, -0.7, -0.5), point3(1.0, 0.4, -1.0), point3(0.0, -0.7, -1.5), material_blue));

    //world.add(make_shared<triangle>(point3(0.0, -0.7, -0.5), point3(0.0, -0.7, -1.5), point3(-1.0, 0.4, -1.0), material_yellow));




    camera cam;

    cam.render(world);

}