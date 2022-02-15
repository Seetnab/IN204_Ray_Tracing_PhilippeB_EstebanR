#ifndef MOTOR4_HPP
#define MOTOR4_HPP


#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "light.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "tools.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>




//Contrairement au précédent moteur, celui-ci permet à la fois de gérer les textures, mais implémente également
//la réflexion et la réfraction des rayons à l'impact, dépendamment des matériaux rencontrés
class motor4: public render{
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
        int height;                 //Hauteur de l'image

    public:
        motor4(){}
        motor4(scene s, scene_lights sl, ambient_light sk, camera c, int w, double r, int ms, int samples, int mr): 
            aScene(s), lights(sl), sky(sk), cam(c), width(w), ratio(r), 
            max_step(ms), nsamples(samples), max_reflection(mr)
                { height = (int) width/ratio;}
        ~motor4(){}

        //Rendu de l'image
        void render_image(){
            std::ofstream ofs("image_motor4.ppm", std::ios::out|std::ios::binary);
            std::vector<color> image(height*width);
            ofs << "P3\n" << width << " " << height << "\n255\n";
            auto start = std::chrono::system_clock::now();
            //Parcours des pixels de l'image
            for (int j = height-1; j>=0; --j){
                for (int i=0; i<width; ++i){
                    color pixel_color;
                    if(nsamples !=1){
                        for(int k=0; k<nsamples; k++){
                            //Envoie de plusieurs rayons par pixel
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

        //Colorisation des pixels
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
                //Si l'objet est un objet lumineux alors renvoie directement la couleur de la lumière
                if(hp.mat->getName()=="light_material"){
                    return hp.rgb;
                }
                //Si l'objet est un mirroir alors on effectue la colorisation sur le rayon réfléchie
                if(hp.mat->getName()=="mirror" && reflected_ray.getDirection()*hp.normal>0){
                    return ray_color(reflected_ray);
                }
                //Calcul des lumières
                lights.hit_lights(hp, aScene, max_step);   
                //Addition des couleurs fournies par les rayons réfractés et réflechis 
                color c=hp.rgb;
                if(refracted_ray.getDirection().norm()>0.01){
                    c = ray_color(refracted_ray);
                    if(reflected_ray.getDirection().norm()>0.01){
                        return (color_multiply(ray_color(reflected_ray),hp.rgb)+c)*0.5;
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

        //Ecriture des données dans le fichier .ppm
        void write_color(std::ofstream &out, std::vector<color> image){
            double ratio_samples = 1.0/nsamples;
            for(int i=0; i<height*width; i++){
                out << (int)(255.999 * sqrt(image[i].getX()*ratio_samples)) << ' '
                    << (int)(255.999 * sqrt(image[i].getY()*ratio_samples)) << ' '
                    << (int)(255.999 * sqrt(image[i].getZ()*ratio_samples)) << '\n';
                }
        }
};



#endif