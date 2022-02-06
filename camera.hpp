# ifndef OBJECTS_HPP
# define OBJECTS_HPP

# include "vec.hpp"

class camera{
    private:
        //Données fournies par l'architecte de la scène
        point origin;               //Point d'origine de la caméra
        point frame_center;         //Point central de la fenêtre de visualisation
        double height;              //Hauteur de la fenêtre capturée par la caméra
        double width;               //Largeur de la fenêtre capturée par la caméra
        //Données calculées
        double focal_length;
        vec direction;
    public:
        //Caméra par défaut
        camera(): origin(0.0,0.0,0.0), frame_center(0.0,0.0,-1.0), height(2.0), width(16.0/9.0*2.0){ vec direction = (frame_center - origin), focal_length = 1.0;}
        //Caméra personnalisée
        camera(point anOrigin, point aFrame_center, double aHeight, double aWidth): origin(anOrigin), frame_center(aFrame_center), 
            height(aHeight), width(aWidth){ vec direction = (frame_center - origin); focal_length = direction.norm(); }
        ~camera(){}
}


# endif