#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include <omp.h>
#include <cmath>
#include <vector>
#include <chrono>

#define MAX_STEP 1000
#define MAX_REFLECTION 10
#define NSAMPLES 100
#define NTHREAD 8


color ray_color(ray& r, scene& aScene) {
    //Couleur des objets dans la scène
    if(r.getMax_reflection()<0){
        return color(0,0,0);
    }
    hit_position hp;
    if(aScene.hit_list(r,hp,MAX_STEP)>0){
        //Réflection aléatoire d'un rayon sur la surface d'un objet
        ray nr(hp.hit_point, hp.normal + rand_unit_vec(), r.getMax_reflection()-1);
        if(r.getMax_reflection() == MAX_REFLECTION){
            return (ray_color(nr,aScene)+hp.rgb)*0.5;
        }
        return ray_color(nr, aScene)*0.5;
    }
    //Couleur du fond
    vec unit_direction = unit_vec(r.getDirection());
    auto t = 0.5*(unit_direction.getY() + 1.0);
    color c=color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
    return c;
}

void write_color(std::ofstream &out, std::vector<color> image, int height, int width, int nSamples) {
    double ratio = 1.0/nSamples;
    // Write the translated [0,255] value of each color component.
    for(int i=0; i<height*width; i++){
        out << (int)(255.999 * image[i].getX()*ratio) << ' '
            << (int)(255.999 * image[i].getY()*ratio) << ' '
            << (int)(255.999 * image[i].getZ()*ratio) << '\n';
    }
}

void render(scene& aScene, int height, int width, camera& cam){
    //Rendu de l'image
    std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);
    std::vector<color> image(height*width);
    ofs << "P3\n" << width << " " << height << "\n255\n";
    auto start = std::chrono::system_clock::now();
    #pragma omp parallel for schedule(static) num_threads(NTHREAD)
    for (int j = height-1; j>=0; --j){
        for (int i=0; i<width; ++i){
            color pixel_color;
            if(NSAMPLES !=1){
                for(int k=0; k<NSAMPLES; k++){
                    double u = double(i + random_n()) / (width-1);
                    double v = double(j + random_n()) / (height-1);
                    ray r = cam.getRay(u,v,MAX_REFLECTION);
                    pixel_color = pixel_color + ray_color(r,aScene);
                }
            }else{
                double u = double(i) / (width-1);
                double v = double(j) / (height-1);
                ray r = cam.getRay(u,v,MAX_REFLECTION);
                pixel_color = ray_color(r,aScene);
            }
            //std::cout << pixel_color << std::endl;
            image[i+(height-1-j)*width] = pixel_color; 
            //std::cout << "(" << i << "," << j << ")" << std::endl;
        }
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> deltaT = end - start;
    std::cout << "Calcul de l'image en " << deltaT.count() << " secondes." << std::endl;
    write_color(ofs, image, height, width, NSAMPLES);
    ofs.close();
}

int main(){
    //remove("image.ppm");
    double ratio = 16.0/9.0;
    int width = 400;
    int height = (int) width/ratio;

    //Création de la scène
    scene aScene;
    sphere s1(point(0,0,-1),1,color(1,0,0));
    sphere s2(point(-1,0,-2),1,color(0,0,1));
    ground g(point(0,-1,0),vec(0,1,0),color(0,1,0));
    aScene.add(&s1);
    aScene.add(&s2);
    aScene.add(&g);

    //Camera
    camera cam(point(0,0,1),point(0,0,-1),2.0,ratio);

    //Rendu de l'image
    
    render(aScene, height, width, cam);
    //std::cerr << "\nDone.\n";
    return 0;
}