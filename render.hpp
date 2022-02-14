#ifndef RENDER_HPP
#define RENDER_HPP

#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include <omp.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <string>



class render{
    public:
        render(){}
        ~render(){}
        virtual void render_image()=0;
        virtual color ray_color(ray& r)=0;
        virtual void write_color(std::ofstream &out, std::vector<color> image)=0;
};


#endif