# ifndef LIGHT_HPP
# define LIGHT_HPP

# include <iostream>
# include <vector>
# include "vec.hpp"
# include "objects.hpp"

class light{
    private:
        point origin;
        color rgb;
        double intensity;
    public:
        light(){}
        light(point anOrigin, color c, double i): origin(anOrigin), rgb(c) , intensity(i){}
        ~light(){}
        color getColor(){
            return rgb;
        }
        point getOrigin(){
            return origin;
        }
        double getIntensity(){
            return intensity;
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
        color hit_lights(hit_position hp, scene aScene, int max){
            color c=hp.rgb;
            for(int i=0; i<(int) list_lights.size(); i++){
                vec light_direction = unit_vec(list_lights[i]->getOrigin() - hp.hit_point);
                ray r(hp.hit_point + hp.normal*0.001, light_direction, 0);
                hit_position nhp;
                if(aScene.hit_list(r, nhp, max)>0){
                    c = c*0.5;
                }else{
                    c = (c +list_lights[i]->getColor()*list_lights[i]->getIntensity())*0.5;
                }
            }
            return c;
        }

};

# endif 