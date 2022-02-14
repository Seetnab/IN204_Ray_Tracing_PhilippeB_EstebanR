#ifndef MOTOR2_HPP
#define MOTOR2_HPP


#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "render.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>



//Moteur moins naif, permettant le déplacement du point du point de vue au travers d'une caméra personnalisable et
//implémentant une première approche pour la diffusion des rayons sur l'ensemble des objets, avec la possibilité de
//mettre en place de l'anti-aliasing en envoyant plusieurs rayons par pixels.
class motor2: public render{
    private:
        scene aScene;               //Ensemble des objets présents sur la scène
        camera cam;                 //Camera décrivant l'angle de vue sur la scène
        int width;                  //Longueur de l'image
        double ratio;               //Ratio de l'image
        int max_step;               //Distance de parcours maximal d'un rayon
        int nsamples;               //Nombre de rayons par pixels
        int max_reflection;         //Nombre de réflexion maximal d'un rayon
        int height;                 //Hauteur de l'image
    public:
        motor2(){}
        motor2(scene s, camera c, int w, double r, int ms, int samples, int mr): 
            aScene(s), cam(c), width(w), ratio(r), max_step(ms), nsamples(samples), max_reflection(mr)
                { height = (int) width/ratio;}
        ~motor2(){}
        void render_image(){
            std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);
            std::vector<color> image(height*width);
            ofs << "P3\n" << width << " " << height << "\n255\n";
            auto start = std::chrono::system_clock::now();
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
            hit_position hp;
            //Couleur des objets dans la scène
            if(aScene.hit_list(r,hp,max_step)>0.001 && r.getMax_reflection() >= 0){
                //Réflection aléatoire d'un rayon sur la surface d'un objet
                vec ndirection = unit_vec(hp.normal + rand_unit_vec());
                ray nr(hp.hit_point, ndirection, r.getMax_reflection()-1);
                return color_multiply(ray_color(nr),hp.rgb);
            }else{
                //Couleur du fond
                vec unit_direction = unit_vec(r.getDirection());
                auto t = 0.5*(unit_direction.getY() + 1.0);
                color c=color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
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