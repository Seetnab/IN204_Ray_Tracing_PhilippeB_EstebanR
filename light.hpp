# ifndef LIGHT_HPP
# define LIGHT_HPP

# include <iostream>
# include <vector>
# include "vec.hpp"
# include "objects.hpp"
# include "material.hpp"
//# include "hit.hpp"



class light{
    protected:
        color rgb;
        double intensity;
    public:
        light(){}
        light(color c, double i): rgb(c), intensity(i){}
        ~light(){}
        virtual color hit_light(hit_position hp, scene aScene, int max)=0;
};

class point_light: public light{
    private:
        point origin;
    public:
        point_light(){}
        point_light(point o, color c, double i): light(c,i), origin(o){}
        ~point_light(){}
        color getColor(){
            return rgb;
        }
        point getOrigin(){
            return origin;
        }
        double getIntensity(){
            return intensity;
        }
        color hit_light(hit_position hp, scene aScene, int max){
            color c;
            vec light_direction = unit_vec(origin - hp.hit_point);
            double distance = (origin - hp.hit_point).norm();
            ray r(hp.hit_point + light_direction*0.01, light_direction, 0);
            hit_position nhp;
            double step = aScene.hit_list(r, nhp, max);
            while(step > 0 && nhp.mat->getName()=="glass"){
                ray nr(r.getOrigin() + r.move(step+0.001), r.getDirection(), r.getMax_reflection());
                step = aScene.hit_list(nr, nhp, max) + 0.001;
                r= nr;
            }
            if(step<=0 || step>=distance){
                double lambert = std::max(r.getDirection()*hp.normal,0.0);
                double dist_dependency = 1/(1+distance);
                c = color_multiply(hp.rgb,rgb)*dist_dependency*lambert*intensity;
            }else{
                c = color_multiply(hp.rgb,color(1,1,1)-rgb)*(1-intensity);
            }
            return c;
        }
};

class sphere_light: public light{
    private:
        sphere spherical;
    public:
        sphere_light(){}
        sphere_light(sphere s, color c, double i): light(c,i), spherical(s){}
        ~sphere_light(){}
        color getColor(){
            return rgb;
        }
        point getCenter(){
            return spherical.getCenter();
        }
        double getRadius(){
            return spherical.getRadius();
        }
        double getIntensity(){
            return intensity;
        }
        color hit_light(hit_position hp, scene aScene, int max){
            color c;
            vec light_direction = unit_vec(spherical.getCenter() - hp.hit_point);
            double distance = (spherical.getCenter() - hp.hit_point).norm()-spherical.getRadius()-0.01;
            ray r(hp.hit_point + light_direction*0.001, light_direction, 0);
            hit_position nhp;
            double step = aScene.hit_list(r, nhp, max);
            //std::cout << step << " " << distance << std::endl;
            while(step > 0 && nhp.mat->getName()=="glass"){
                ray nr(r.getOrigin() + r.move(step+0.001), r.getDirection(), r.getMax_reflection());
                step = aScene.hit_list(nr, nhp, max) + 0.001;
                r= nr;
            }
            if(step<=0 || step>distance){
                double lambert = std::max(r.getDirection()*hp.normal,0.0);
                double dist_dependency = 1/(1+distance);
                c = color_multiply(hp.rgb,rgb)*dist_dependency*lambert*intensity;
            }else{
                c = color_multiply(hp.rgb,color(1,1,1)-rgb)*(1-intensity);
            }
            return c;
        }
};


class ambient_light: public light{
    private:
        vec direction;
    public:
        ambient_light(){}
        ambient_light(vec aDirection, color c, double i): light(c,i), direction(unit_vec(aDirection)){}
        ~ambient_light(){}
        color getColor(){
            return rgb;
        }
        point getDirection(){
            return direction;
        }
        double getIntensity(){
            return intensity;
        }
        color hit_light(hit_position hp, scene aScene, int max){
            color c;
            ray r(hp.hit_point + hp.normal*0.001, direction*(-1), 0);
            hit_position nhp;
            double step = aScene.hit_list(r, nhp, max);
            while(step > 0 && nhp.mat->getName()=="glass"){
                ray nr(r.getOrigin() + r.move(step+0.001), r.getDirection(), r.getMax_reflection());
                step = aScene.hit_list(nr, nhp, max) + 0.001;
                r= nr;
            }
            if(step<0){
                double lambert = std::max(r.getDirection()*hp.normal,0.0);
                c = color_multiply(hp.rgb,rgb)*lambert*intensity;

            }else{
                c = color_multiply(hp.rgb,color(1,1,1)-rgb)*(1-intensity);
            }
            return c;
        }
};

class scene_lights{
    private:
        std::vector<light*> list_lights;
    public:
        scene_lights(){}
        ~scene_lights(){}
        void add(light* aLight){
            list_lights.push_back(aLight);
        }
        void clear(){
            list_lights.clear();
        }
        void hit_lights(hit_position& hp, scene aScene, int max){
            color c;
            for(int i=0; i<(int) list_lights.size(); i++){
                c = c + list_lights[i]->hit_light(hp,aScene,max);
            }
            hp.rgb = color_max(c);
        }

};

# endif 