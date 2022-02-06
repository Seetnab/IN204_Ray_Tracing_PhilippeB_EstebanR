#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include <cmath>

#define MAX_STEP 100
#define MAX_REFLECTION 5


color ray_color(ray& r, scene& aScene) {
    //Couleur des objets dans la scène
    hit_position hp;
    if(aScene.hit_list(r,hp,MAX_STEP)>=0){
        return hp.rgb;
    }

    //Couleur du fond
    vec unit_direction = unit_vector(r.getDirection());
    //std::cout << unit_direction;
    auto t = 0.5*(unit_direction.getY() + 1.0);
    //std::cout << t;
    color c=color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
    //std::cout << c;
    return c;
}

void write_color(std::ofstream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << (int)(255.999 * pixel_color.getX()) << ' '
        << (int)(255.999 * pixel_color.getY()) << ' '
        << (int)(255.999 * pixel_color.getZ()) << '\n';
}

int main(){
    //remove("image.ppm");
    double ratio = 16.0/9.0;
    int width = 400;
    int height = (int) width/ratio;

    //Création de la scène
    scene aScene;
    sphere s1(point(0,0,-1),0.5,color(1,0,0));
    sphere s2(point(-1,0,-2),0.5,color(0,0,1));
    ground g(point(0,-1,0),vec(0,1,0),color(0,1,0));
    aScene.add(&s1);
    aScene.add(&s2);
    aScene.add(&g);

    //Camera
    camera cam(point(0,1,200),point(0,0,-1),2.0,ratio);

    //Rendu de l'image
    std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height-1; j>=0; --j){
        for (int i=0; i<width; ++i){
            double u = double(i) / (width-1);
            double v = double(j) / (height-1);
            ray r = cam.getRay(u,v,MAX_REFLECTION);
            color pixel_color = ray_color(r,aScene);
            write_color(ofs, pixel_color);
        }
    }
    //std::cerr << "\nDone.\n";
}