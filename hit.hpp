# ifndef HIT_HPP
# define HIT_HPP

#include "vec.hpp"
#include "material.hpp"

//Structure contenant le point d'intersection entre un objet et un rayon, et la normal au point de collision
struct hit_position{
    point hit_point;
    vec normal;
    color rgb;
    material mat;
};

#endif