#ifndef MOTOR1_HPP
#define MOTOR1_HPP


#include "vec.hpp"
#include "objects.hpp"
#include "render.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>



//Moteur basique, permettant d'afficher l'ensemble des objets de la scène, avec leur couleur, sans lumière, ombre ou matériau.
//En somme, un moteur de tracé de rayon sans réelle interaction avec les objets, sans changement de position possible
class motor1: public render{
    private:
        scene aScene;               //Ensemble des objets présents sur la scène
        int width;                  //Longueur de l'image
        double ratio;               //Ratio de l'image
        int max_step;               //Distance de parcours maximal d'un rayon
        int height;                 //Hauteur de l'image
    public:
        motor1(){}
        motor1(scene s, int w, double r, int ms): aScene(s), width(w), ratio(r), max_step(ms){ height = (int) width/ratio;}
        ~motor1(){}
        void render_image(){
            double view_height = 2.0;
            double view_width = ratio*view_height;
            double focal = 1.0;

            point origin = point(0,0,0);
            vec horizontal = vec(view_width, 0, 0);
            vec vertical = vec(0, view_height, 0);
            point lw_lft_corner = origin - horizontal/2 - vertical/2 - vec(0, 0, focal);
            //Rendu de l'image
            auto start = std::chrono::system_clock::now();
            std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);
            std::vector<color> image(height*width);
            ofs << "P3\n" << width << " " << height << "\n255\n";
            for (int j = height-1; j>=0; --j){
                for (int i=0; i<width; ++i){
                    double u = double(i) / (width-1);
                    double v = double(j) / (height-1);
                    vec dir = (vec) lw_lft_corner + horizontal*u + vertical*v - origin;
                    ray r(origin, dir, 1);
                    color pixel_color = ray_color(r);
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
            if(aScene.hit_list(r,hp,max_step)>=0){
                return hp.rgb;
            }
            //Couleur du fond
            vec unit_direction = unit_vec(r.getDirection());
            auto t = 0.5*(unit_direction.getY() + 1.0);
            color c=color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
            return c;
        }
        void write_color(std::ofstream &out, std::vector<color> image){
           // Write the translated [0,255] value of each color component.
            for(int i=0; i<height*width; i++){
                out << (int)(255.999 * image[i].getX()) << ' '
                    << (int)(255.999 * image[i].getY()) << ' '
                    << (int)(255.999 * image[i].getZ()) << '\n';
            }
        };
};



#endif