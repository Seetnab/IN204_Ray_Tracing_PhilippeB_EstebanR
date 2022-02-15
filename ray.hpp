#ifndef RAY_HPP
#define RAY_HPP

#include "vec.hpp"

//Création d'une classe rayon, ayant un point d'origine et un vecteur direction
class ray{
    private:
        point origin;
        vec direction;
        //Nombre max de reflexion d'un rayon
        int max_reflection;
    public:
        ray(){}
        ray(point anOrigin, vec aDirection, int aMax_reflection) : origin(anOrigin), direction(aDirection), max_reflection(aMax_reflection){}
        ray(const ray &aVector) : origin(aVector.origin), direction(aVector.direction), max_reflection(aVector.max_reflection){}
        ~ray(){}
        //Méthodes de lecture
        point getOrigin() const{
            return origin;
        }
        vec getDirection() const{
            return direction;
        }
        int getMax_reflection() const {
            return max_reflection;
        }
        //Méthode de déplacement le long du rayon
        point move(double step){
            return origin + direction*step;
        }
};


#endif