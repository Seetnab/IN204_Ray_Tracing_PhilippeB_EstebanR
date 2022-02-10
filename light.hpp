# ifndef LIGHT_HPP
# define LIGHT_HPP

# include <iostream>
# include <vector>
# include "vec.hpp"
# include "objects.hpp"


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
            if(step<0 || step>distance){
                double lambert = std::max(r.getDirection()*hp.normal,0.0);
                double dist_dependency = 1/(1+distance);
                c = color_multiply(hp.rgb,rgb)*dist_dependency*lambert*intensity;
            }else{
                c = color(0,0,0);
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
            if(aScene.hit_list(r, nhp, max)<0){
                double lambert = std::max(r.getDirection()*hp.normal,0.0);
                c = color_multiply(hp.rgb,rgb)*lambert*intensity;

            }else{
                c = color(0,0,0);
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
                //std::cout << i << std::endl;
            }
            hp.rgb = c;
        }

};

# endif 