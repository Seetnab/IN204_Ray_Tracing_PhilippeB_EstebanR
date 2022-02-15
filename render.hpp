#ifndef RENDER_HPP
#define RENDER_HPP

#include <iostream>
#include <fstream>
# include "ray.hpp"


//Classe mère permettant la conception de plusieurs moteurs de rendu d'image par ray tracing
class render{
    public:
        render(){}
        ~render(){}
        virtual void render_image()=0;
        virtual color ray_color(ray& r)=0;
        virtual void write_color(std::ofstream &out, std::vector<color> image)=0;
};


#endif