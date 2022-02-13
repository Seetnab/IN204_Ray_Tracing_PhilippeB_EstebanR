#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>
#include <string>
#include "vec.hpp"


struct hit_position;

class material{
    protected:
        std::string name;
    public:
        material(){}
        material(std::string aName): name(aName){}
        ~material(){}
        virtual void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray)=0;
        std::string getName(){
            return name;
        }
};

class default_material: public material{
    public:
        default_material(): material("default"){}
        ~default_material(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            reflected_ray = ray(point(0,0,0), vec(0,0,0), -1);
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
        }
};

class diffuse_material: public material{
    public:
        diffuse_material(): material("diffuse"){}
        ~diffuse_material(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            vec reflection_direction = unit_vec(hp.normal + rand_unit_vec());
            reflected_ray = ray(hp.hit_point, reflection_direction, r.getMax_reflection()-1);
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
        }
};

class metal: public material{
    private:
    public:
        metal(): material("metal"){}
        ~metal(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
            vec reflection_direction = unit_vec(r.getDirection() - hp.normal*2*(r.getDirection()*hp.normal));
            reflected_ray = ray(hp.hit_point + reflection_direction*0.01,reflection_direction, r.getMax_reflection()-1);
        }
};

class mirror: public material{
    private:
    public:
        mirror(): material("mirror"){}
        ~mirror(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
            vec reflection_direction = unit_vec(r.getDirection() - hp.normal*2*(r.getDirection()*hp.normal));
            reflected_ray = ray(hp.hit_point + reflection_direction*0.01,reflection_direction, r.getMax_reflection()-1);
        }
};

class glass: public material{
    private:
        double opacity;
        double glass_index;
    public:
        glass(): material("glass"), glass_index(1.3){}
        glass(double gi): material("glass"), glass_index(gi){}
        ~glass(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            ray nr;
            double cos = hp.normal*r.getDirection();
            //std::cout << angle << std::endl;
            if(cos<0){
                vec refracted_perp = (r.getDirection()-hp.normal*cos)*(1.0/glass_index);
                vec refracted_para = hp.normal*sqrt(r.getDirection().norm()*r.getDirection().norm()-refracted_perp.norm()*refracted_perp.norm())*(-1);
                vec refracted_direction = refracted_para + refracted_perp;
                refracted_ray = ray(hp.hit_point + refracted_direction*0.001, refracted_direction, r.getMax_reflection()-1);
            }else{
                vec refracted_perp = (r.getDirection()-hp.normal*cos)*(glass_index);
                vec refracted_para = hp.normal*sqrt(1-refracted_perp.norm()*refracted_perp.norm());
                refracted_ray = ray(hp.hit_point, unit_vec(refracted_perp + refracted_para), r.getMax_reflection()-1);
                std::cout << refracted_ray.getDirection().norm() << std::endl;
            }
            reflected_ray = ray(point(0,0,0), vec(0,0,0), -1);
        }
};

#endif