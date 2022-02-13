# ifndef OBJECTS_HPP
# define OBJECTS_HPP

# include <iostream>
# include <vector>
# include <math.h>
# include "vec.hpp"


class material; 

struct hit_position{
    point hit_point;
    vec normal;
    color rgb;
    material* mat;
};

//Classe mère contenant les méthodes et attributs commun à tous les objets de la scène
class scene_basic_object{
    //Permet l'accès à l'attribut aux classes filles
    protected:
        color rgb;
    public:
        scene_basic_object(){}
        scene_basic_object(color aColor) : rgb(aColor){}
        ~scene_basic_object(){}
        virtual double hit_object(ray& r,struct hit_position& intersect)=0;

};

//Classe fille objet permettant la création d'une sphère
class sphere: public scene_basic_object{
    private:
        point center;
        double radius;
        material* mat;
    public:
        sphere(){}
        //Mettre le constructeur de la classe mère en premier pour faire appel au bon constructeur
        sphere(point aCenter, double aRadius, color aColor,material* m) :
            scene_basic_object(aColor), center(aCenter), radius(aRadius), mat(m){}
        ~sphere(){}
        point getCenter() const{
            return center;
        }
        double getRadius() const{
            return radius;
        }
        color getColor() const{
            return rgb;
        }
        //Renvoi step, qui est la distance entre le point d'origine du rayon et le point d'impact
        double hit_object(ray& r, struct hit_position& intersect){
            double A = r.getDirection()*r.getDirection();
            double B = r.getDirection()*(r.getOrigin()-center)*2.0;
            double C = (r.getOrigin()-center)*(r.getOrigin()-center) - radius*radius;
            double delta = B*B - 4*A*C;
            if(delta >= 0){
                double step = (-B-sqrt(delta))/(2*A);
                intersect.hit_point = r.move(step);
                intersect.normal = (intersect.hit_point - center)/radius;
                intersect.rgb = rgb;
                intersect.mat = mat;
                return step;
            }
            return -1.0;
        }
};

//Classe fille objet permettant la création d'une surface plane (sol)
class ground: public scene_basic_object{
    private:
        point origin;
        vec normal;
        material* mat;
    public:
        ground(){}
        //Mettre le constructeur de la classe mère en premier pour faire appel au bon constructeur
        ground(point anOrigin, vec aNormal, color aColor, material* m) :
            scene_basic_object(aColor),origin(anOrigin), normal(aNormal), mat(m){}
        ~ground(){}
        //Renvoi step, qui est la distance entre le point d'origine du rayon et le point d'impact
        double hit_object(ray& r, struct hit_position& intersect){
            double delta = normal*r.getDirection();
            if(delta!=0){
                double num = normal*(origin - r.getOrigin());
                double step = num/delta;
                intersect.hit_point = r.move(step);
                intersect.normal = normal;
                intersect.rgb = rgb;
                intersect.mat = mat;
                return step;
            }
            return -1.0;
        };
};

//Définiton de la classe rectangle, qui peut être vue comme un miroir ou une fenêtre 
class rectangle: public scene_basic_object{
    private:
        point origin;
        vec normal;
        double width;
        double height;
        material* mat;
    public:
        rectangle(){}
        rectangle(point anOrigin, vec aNormal, double aWidth, double aHeight, color aColor, material* m) :
            scene_basic_object(aColor), origin(anOrigin), normal(aNormal), width(aWidth), height(aHeight), mat(m){}
        ~rectangle(){}
        double hit_object(ray& r, struct hit_position& intersect){
            double delta = normal*r.getDirection();
            if(delta!=0){
                double num = normal*(origin - r.getOrigin());
                double step = num/delta;
                point impact = r.move(step);
                if(impact.getX()>=origin.getX() && impact.getX()<=(origin.getX()+width) && impact.getY()>=origin.getY() && impact.getY()<=(origin.getY()+height)){
                    intersect.hit_point = r.move(step);
                    intersect.normal = normal;
                    intersect.rgb = rgb;
                    intersect.mat = mat;
                    return step;
                }
            }
            return -1.0;
        }
};
 
//Classe regroupant l'ensemble des objets présent sur scène
class scene{
    private:
        std::vector<scene_basic_object*> list_objects;
    public:
        scene(){}
        scene(std::vector<scene_basic_object*> aList_objects) : list_objects(aList_objects){}
        scene(const scene &aScene) : list_objects(aScene.list_objects){}
        ~scene(){}
        std::vector<scene_basic_object*> getList(){
            return list_objects;
        }
        void add(scene_basic_object* new_object){
            list_objects.push_back(new_object);
        }
        void clear(){
            list_objects.clear();
        }
        double hit_list(ray& r, struct hit_position& intersect, double max_step){
            hit_position tmp;
            double step = max_step;
            for(int i=0; i<(int)list_objects.size();i++){
                double res = list_objects[i]->hit_object(r,tmp);
                if(res < step && res>0){
                    intersect = tmp;
                    step = res;
                }
            }
            if(step == max_step){
                return -1.0;
            }
            return step;
        }
};

# endif 