#ifndef MOTOR5_HPP
#define MOTOR5_HPP


#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "light.hpp"
#include "material.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>



//Même moteur que précedemment mais avec parallélisation OMP
class motor5: public render{
    private:
        scene aScene;               //Ensemble des objets présents sur la scène
        scene_lights lights;        //Ensemble des lumières de la scène
        ambient_light sky;          //Lumière du ciel
        camera cam;                 //Camera décrivant l'angle de vue sur la scène
        int width;                  //Longueur de l'image
        double ratio;               //Ratio de l'image
        int max_step;               //Distance de parcours maximal d'un rayon
        int nsamples;               //Nombre de rayons par pixels
        int max_reflection;         //Nombre de réflexion maximal d'un rayon
        int nthread;                //Nombre de threads OMP
        int height;                 //Hauteur de l'image

    public:
        motor5(){}
        motor5(scene s, scene_lights sl, ambient_light sk, camera c, int w, double r, int ms, int samples, int mr, int nt): 
            aScene(s), lights(sl), sky(sk), cam(c), width(w), ratio(r), 
            max_step(ms), nsamples(samples), max_reflection(mr), nthread(nt)
                { height = (int) width/ratio;}
        ~motor5(){}
        void render_image(){
            std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);
            std::vector<color> image(height*width);
            ofs << "P3\n" << width << " " << height << "\n255\n";
            auto start = std::chrono::system_clock::now();
            #pragma omp parallel for schedule(dynamic) num_threads(nthread)
            for (int j = height-1; j>=0; --j){
                for (int i=0; i<width; ++i){
                    color pixel_color;
                    if(nsamples !=1){
                        for(int k=0; k<nsamples; k++){
                            double u = double(i + random_n(0,1)) / (width-1);
                            double v = double(j + random_n(0,1)) / (height-1);
                            ray r = cam.getRay(u,v,max_reflection);
                            pixel_color = pixel_color + ray_color(r);
                        }
                    }else{
                        double u = double(i) / (width-1);
                        double v = double(j) / (height-1);
                        ray r = cam.getRay(u,v,max_reflection);
                        pixel_color = ray_color(r);
                    }
                    image[i+(height-1-j)*width] = pixel_color; 
                }
            }
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> deltaT = end - start;
            std::cout << "Calcul de l'image en " << deltaT.count() << " secondes." << std::endl;
            write_color(ofs, image);
            ofs.close();
        }
        color ray_color(ray& r){
            //Couleur des objets dans la scène
            hit_position hp;
            if(r.getMax_reflection()<0){
                return color(0,0,0);
            }
            if(aScene.hit_list(r,hp,max_step)>0.001){
                //Réflexion et réfraction du rayon incident en fonction des matériaux touché
                ray reflected_ray;
                ray refracted_ray;
                hp.mat->ray_interaction(r, hp, reflected_ray, refracted_ray);
                if(hp.mat->getName()=="light_material"){
                    return hp.rgb;
                }
                if(hp.mat->getName()=="mirror" && reflected_ray.getDirection()*hp.normal>0){
                    return ray_color(reflected_ray);
                }
                lights.hit_lights(hp, aScene, max_step);   
                color c=hp.rgb;
                if(refracted_ray.getDirection().norm()>0.01){
                    c = ray_color(refracted_ray)*hp.mat->getTransmittance();
                    if(reflected_ray.getDirection().norm()>0.01){
                        return (color_multiply(ray_color(reflected_ray),hp.rgb)*hp.mat->getReflectance()+c)*0.5;
                    }
                    return c;
                }if(reflected_ray.getDirection().norm()>0.01){
                    return (color_multiply(ray_color(reflected_ray),hp.rgb)+c)*0.5;
                }
                return color_max((c+hp.rgb)*0.5);
            }else{
                //Couleur du fond
                vec unit_direction = unit_vec(r.getDirection());
                auto t = 0.5*(unit_direction*sky.getDirection() + 1.0);
                color c = (sky.getColor()*(1.0-t)
                    + color_multiply(color(0.2, 0.4, 1.0), sky.getColor()*t))*sky.getIntensity();
                return color_max(c);
            }
        }
        void write_color(std::ofstream &out, std::vector<color> image){
            double ratio_samples = 1.0/nsamples;
            // Write the translated [0,255] value of each color component.
            for(int i=0; i<height*width; i++){
                out << (int)(255.999 * sqrt(image[i].getX()*ratio_samples)) << ' '
                    << (int)(255.999 * sqrt(image[i].getY()*ratio_samples)) << ' '
                    << (int)(255.999 * sqrt(image[i].getZ()*ratio_samples)) << '\n';
                }
        }
};



#endif