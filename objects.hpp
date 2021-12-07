# ifndef OBJECTS_HPP
# define OBJECTS_HPP

# include <iostream>
# include <stdbool.h>
# include <vector>
# include <math.h>
# include "vec.hpp"



//Structure contenant le point d'intersection entre un objet et la normal au point de collision
struct hit_position{
    point hit_point;
    vec normal;
};

//Classe mère contenant les méthodes et attributs commun à tous les objets de la scène
class scene_basic_object{
    private:
    public:
        scene_basic_object(){}
        ~scene_basic_object(){}
        virtual struct hit_position hit_object(ray r)=0;

};

//Classe fille objet permettant la création d'une sphère
class sphere: public scene_basic_object{
    private:
        point center;
        double radius;
    public:
        sphere(){}
        sphere(point aCenter, double aRadius) : center(aCenter), radius(aRadius){}
        sphere(sphere aSphere) : center(aSphere.center), radius(aSphere.radius){}
        ~sphere(){}
        double hit_object(ray r, struct hit_point* intersect){
            double A = r.getDirection()*r.getDirection();
            double B = 2.0*r.getDirection()*(r.getOrigin()-center);
            double C = (r.getOrigin()-center)*(r.getOrigin-center) - radius*radius;
            double delta = B*B - 4*A*C;
            if(delta >= 0){
                double step = (-B-sqrt(delta))/(2*A);
                intersect->hit_point = r.move(step);
                intersect->normal = (intersect->hit_point - center)/radius;
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
    public:
        ground(){}
        ground(point anOrigin, vec aNormal) : origin(anOrigin), normal(aNormal){}
        ~ground(){}
        double hit_position hit_object(ray r, struct hit_point* intersect){
            double delta = normal*r.getDirection();
            if(delta!=0){
                double num = normal*(origin - r.getOrigin());
                double step = num/delta;
                intersect->hit_point = r.move(step);
                intersect->normal = normal;
                return step;
            }
            return -1.0;
        };
};
 
//Classe regroupant l'ensemble des objets présent sur la scène
class scene{
    private:
        std::vector<scene_basic_object*> list_objects;
    public:
        scene(){}
        scene(vector::std<scene_basic_object*> aList_objects) : list_objects(aList_objects){}
        scene(scene aScene) : list_objects(aScene.list_objects){}
        ~scene(){}
};

# endif 