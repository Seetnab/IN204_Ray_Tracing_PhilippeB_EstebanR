#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "vec.hpp"


//Renvoie un nombre aléatoire dans l'intervalle [min,max]
double random_n(double min, double max){
    return min + (max-min)*(double)(rand()) / (double)(RAND_MAX);
}

//Renvoie un vecteur unité
vec unit_vec(vec v) {
    return v / v.norm();
}


//Renvoie un vecteur aléatoire au sein d'une sphère unitaire
vec rand_unit_vec(){
    vec v(1,1,1);
    while(v.norm()>=1.0){
        v = vec(random_n(-1,1),random_n(-1,1),random_n(-1,1));
    }
    return unit_vec(v);
}


//Permet la multiplication des coordonnées de 2 couleurs
color color_multiply(color c1, color c2){
    return color(c1.getX()*c2.getX(), c1.getY()*c2.getY(), c1.getZ()*c2.getZ());
}


//Permet d'assurer le bon format des couleurs
color color_max(color c){
    color nc = c;
    if(c.getX()<0){
        nc.setX(0);
    }if(c.getX()>1){
        nc.setX(1);
    }
    if(c.getY()<0){
        nc.setY(0);
    }if(c.getY()>1){
        nc.setY(1);
    }if(c.getZ()<0){
        nc.setZ(0);
    }if(c.getZ()>1){
        nc.setZ(1);
    }
    return nc;
}

#endif