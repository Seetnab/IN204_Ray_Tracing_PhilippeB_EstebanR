# ifndef OBJECTS_HPP
# define OBJECTS_HPP

# include <iostream>
# include <vector>
# include <math.h>
# include "vec.hpp"



//Structure contenant le point d'intersection entre un objet et un rayon, et la normal au point de collision
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
        virtual double hit_object(ray r,struct hit_position* intersect)=0;

};

//Classe fille objet permettant la création d'une sphère
class sphere: public scene_basic_object{
    private:
        point center;
        double radius;
    public:
        sphere(){}
        sphere(point aCenter, double aRadius) : scene_basic_object() {center = aCenter; radius = aRadius;}
        sphere(const sphere& aSphere) : center(aSphere.center), radius(aSphere.radius){}
        ~sphere(){}
        point getCenter() const{
            return center;
        }
        double getRadius() const{
            return radius;
        }
        //Renvoi step, qui est la distance entre le point d'origine du rayon et le point d'impact
        double hit_object(ray r, struct hit_position* intersect){
            double A = r.getDirection()*r.getDirection();
            double B = r.getDirection()*(r.getOrigin()-center)*2.0;
            double C = (r.getOrigin()-center)*(r.getOrigin()-center) - radius*radius;
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
        //Renvoi step, qui est la distance entre le point d'origine du rayon et le point d'impact
        double hit_object(ray r, struct hit_position* intersect){
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
        double hit_list(ray r, struct hit_position* intersect, double max_step){
            hit_position tmp;
            double step = max_step;
            for(int i=0; i<(int)list_objects.size();i++){
                double res = list_objects[i]->hit_object(r,&tmp);
                if(res!=-1.0 && res < step){
                    *intersect = tmp;
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