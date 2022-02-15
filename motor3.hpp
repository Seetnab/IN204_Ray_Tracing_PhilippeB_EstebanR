#ifndef MOTOR3_HPP
#define MOTOR3_HPP


#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "tools.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>



//Moteur qui prend cette fois-ci en compte les rayons lumineux au travers d'une méthode de traçage des rayons d'ombres
//à chaque impact sur un objet, ainsi que de gérer une lumière d'ambiance dont la coulour background dépend.
//Ce moteur ne supporte pas encore les types de matériaux, d'où l'étrangeté de la boule lumineuse
class motor3: public render{
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
        motor3(){}
        motor3(scene s, scene_lights sl, ambient_light sk, camera c, int w, double r, int ms, int samples, int mr): 
            aScene(s), lights(sl), sky(sk), cam(c), width(w), ratio(r), 
            max_step(ms), nsamples(samples), max_reflection(mr)
                { height = (int) width/ratio;}
        ~motor3(){}

        //Rendu de l'image
        void render_image(){
            std::ofstream ofs("image_motor3.ppm", std::ios::out|std::ios::binary);
            std::vector<color> image(height*width);
            ofs << "P3\n" << width << " " << height << "\n255\n";
            auto start = std::chrono::system_clock::now();
            //Parcours des pixels de l'image
            for (int j = height-1; j>=0; --j){
                for (int i=0; i<width; ++i){
                    color pixel_color;
                    if(nsamples !=1){
                        //Envoie de plusieurs rayons par pixel
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

        //Colorisation des pixels
        color ray_color(ray& r){
            //Couleur des objets dans la scène
            hit_position hp;
            if(aScene.hit_list(r,hp,max_step)>0.001 && r.getMax_reflection() >= 0){
                //Réflection aléatoire d'un rayon sur la surface d'un objet
                vec ndirection = unit_vec(hp.normal + rand_unit_vec());
                ray nr(hp.hit_point, ndirection, r.getMax_reflection()-1);
                //Calcul des lumières
                lights.hit_lights(hp, aScene, max_step);
                color c = hp.rgb;   
                return (color_multiply(ray_color(nr),hp.rgb)+c)*0.5;
            }else{
                //Couleur du fond
                vec unit_direction = unit_vec(r.getDirection());
                auto t = 0.5*(unit_direction*sky.getDirection() + 1.0);
                color c = (sky.getColor()*(1.0-t)
                    + color_multiply(color(0.2, 0.4, 1.0), sky.getColor()*t))*sky.getIntensity();
                return c;
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