#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>
#include <string>
#include "vec.hpp"


struct hit_position;

class material{
    protected:
        std::string name;
        double reflectance;
        double transmittance;
    public:
        material(){}
        material(std::string aName, double r, double t): name(aName), reflectance(r), transmittance(t){}
        ~material(){}
        virtual void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray)=0;
        std::string getName(){
            return name;
        }
        double getReflectance(){
            return reflectance;
        }
        double getTransmittance(){
            return transmittance;
        }
        
};

class default_material: public material{
    private:
    public:
        default_material(): material("default",0,0){}
        ~default_material(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            reflected_ray = ray(point(0,0,0), vec(0,0,0), -1);
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
        }
};

class diffuse_material: public material{
    private:
    public:
        diffuse_material(): material("diffuse",1,0){}
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
        metal(): material("metal",1,0){}
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
        mirror(): material("mirror",1,0){}
        ~mirror(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
            vec reflection_direction = unit_vec(r.getDirection() - hp.normal*2*(r.getDirection()*hp.normal));
            reflected_ray = ray(hp.hit_point + reflection_direction*0.01,reflection_direction, r.getMax_reflection()-1);
        }
};

class glass: public material{
    private:
        double glass_index;
    public:
        glass(): material("glass",0,1), glass_index(1.5){}
        glass(double r, double t): material("glass",r,t), glass_index(1.5){}
        ~glass(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            double cos = hp.normal*r.getDirection();
            reflected_ray = ray(point(0,0,0), vec(0,0,0), -1);
            vec refracted_perp;
            vec refracted_para;
            if(cos<0){
                refracted_perp = (r.getDirection()-hp.normal*cos)*(1.0/glass_index);
            }
            else{
                refracted_perp = (r.getDirection()+hp.normal*cos)*(1.0/glass_index);
            }
            double para = r.getDirection().norm()*r.getDirection().norm()-refracted_perp.norm()*refracted_perp.norm();
            vec reflection_direction = unit_vec(r.getDirection() - hp.normal*2*(r.getDirection()*hp.normal));
            reflected_ray = ray(hp.hit_point + reflection_direction*0.01,reflection_direction, r.getMax_reflection()-1);
            if(para>=0){
                refracted_para = hp.normal*sqrt(para)*(-1);
                vec refracted_direction = refracted_para + refracted_perp;
                refracted_ray = ray(hp.hit_point + refracted_direction*0.001, refracted_direction, r.getMax_reflection()-1);
            }
        }
};

class light_material: public material{
    private:
    public:
        light_material(): material("light_material",0,0){}
        ~light_material(){}
        void ray_interaction(ray& r, hit_position& hp, ray& reflected_ray, ray& refracted_ray){
            reflected_ray = ray(point(0,0,0), vec(0,0,0), -1);
            refracted_ray = ray(point(0,0,0), vec(0,0,0), -1);
        }
};

#endif