#ifndef MOTOR2_HPP
#define MOTOR2_HPP

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

class motor2: public render{
    public:
        motor2(){}
        motor2(int mstep, int meref, int samples, int thread): render(int mstep, int meref, int samples, int thread){}
        ~motor2(){}
        virtual void render_image(scene& aScene, scene_lights lights, int height, int width, camera& cam, ambient_light sky){

        };
        virtual color ray_color(ray& r, scene& aScene, scene_lights lights, ambient_light sky){

        };
        virtual void write_color(std::ofstream &out, std::vector<color> image, int height, int width, int nSamples){

        };
};



#endif