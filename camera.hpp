# ifndef CAMERA_HPP
# define CAMERA_HPP

# include "vec.hpp"

class camera{
    public:
        //Données fournies par l'architecte de la scène
        point origin;               //Point d'origine de la caméra
        point frame_center;         //Point central de la fenêtre de visualisation
        double view_height;              //Hauteur de la fenêtre capturée par la caméra
        double view_width;               //Largeur de la fenêtre capturée par la caméra
        double ratio;
        //Données calculées
        double focal_length;
        vec horizontal;
        vec vertical;
        point lower_left_corner;
    public:
        //Caméra par défaut
        camera(){
            origin = point(0,0,0);
            ratio = 16.0/9.0;
            view_height = 2.0;
            view_width = ratio*view_height;
            focal_length = 1.0; 
            horizontal = vec(view_width, 0, 0);
            vertical = vec(0, view_height, 0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - vec(0, 0, focal_length);
            }
        //Caméra personnalisée
        camera(point anOrigin, point aFrame_center, double aHeight, double aRatio){
            origin = anOrigin;
            frame_center = aFrame_center;
            ratio = aRatio;
            view_height = aHeight;
            view_width = ratio*view_height;

            //Définition du plan de la caméra
            vec direction = unit_vec(frame_center - origin);
            vec h;
            if(direction.getX()==0 && direction.getZ()==0){
                h = unit_vec(direction.prod_vect(vec(1,0,0)));
            }else{
                h = unit_vec(direction.prod_vect(vec(0,1,0)));
            }
            vec v = unit_vec(h.prod_vect(direction));
            //Calcul de l'espace limité de la caméra
            focal_length = direction.norm(); 
            horizontal = h*view_width;
            vertical = v*view_height;
            lower_left_corner = origin - horizontal/2 - vertical/2 + direction;
        }
        ~camera(){}
        ray getRay(double u, double v, int max_reflection){
            vec dir = (vec) lower_left_corner + horizontal*u + vertical*v - origin;
            return ray(origin, dir, max_reflection);
        }
};


# endif