#ifndef DESCRIPTION_HPP
#define DESCRIPTION_HPP

#include "material.hpp"
#include "objects.hpp"
#include "light.hpp"
#include "vec.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "tools.hpp"

void render_scene1(scene& aScene, scene_lights& lights, ambient_light& sky, camera& cam, int& width, double& ratio ){
    //Liste des matériaux
    diffuse_material d;
    default_material dd;
    metal me;
    mirror mi;
    glass gl;
    light_material lm;

    //Création et ajout des objets dans la scène
    sphere s1(point(3,0,-1),1,color(1,0,0),&me);
    sphere s2(point(-3,0,-2),1,color(1,1,1),&mi);
    sphere s3(point(-4.5,0,-5),1,color(0,0,1),&d);
    sphere s4(point(0,0,-7),1,color(0,0,1),&d);
    sphere s5(point(0,-0.6,-1),0.4,color(1,0.1,0.1),&lm);
    ground g(point(0,-1,0),vec(0,1,0),color(0.1,0.8,0.2),&d);
    rectangle r(point(-2.5,-1,-5.5),vec(0,0,1),5,5,color(0.5,0.5,0.5),&gl);
    aScene.add(&s1);
    aScene.add(&s2);
    aScene.add(&s3);
    aScene.add(&s4);
    aScene.add(&s5);
    aScene.add(&g);
    aScene.add(&r);

    //Création et ajout des lumières dans la scène
    sky = ambient_light(vec(0,-1,0), color(1,1,1),1);
    point_light l2(point(-2,-0.99,-1),color(1,1,1),0.9);
    point_light l3(point(0,1,-2),color(1,1,1),0.9);
    sphere_light sl(s5, 1);
    lights.add(&sky);
    lights.add(&l2);
    lights.add(&l3);
    lights.add(&sl);

    //Positionnement de la caméra et définition de l'écran
    width = 1200;
    ratio = 16.0/9.0;
    cam = camera(point(0,0.5,1),point(0,0,-1),2.0,ratio);
}


#endif