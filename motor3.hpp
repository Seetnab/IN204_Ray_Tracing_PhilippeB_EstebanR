#ifndef MOTOR1_HPP
#define MOTOR1_HPP

#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "render.hpp"
#include <omp.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <string>

class motor3: public render{
    public:
        motor3(){}
        motor3(int mstep, int meref, int samples, int thread): render(int mstep, int meref, int samples, int thread){}
        ~motor3(){}
        virtual void render_image(scene& aScene, scene_lights lights, int height, int width, camera& cam, ambient_light sky){

        };
        virtual color ray_color(ray& r, scene& aScene, scene_lights lights, ambient_light sky){

        };
        virtual void write_color(std::ofstream &out, std::vector<color> image, int height, int width, int nSamples){

        };
};



#endif