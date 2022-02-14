#include <fstream>
#include <iostream>
#include "motor1.hpp"
#include "motor2.hpp"
#include "motor3.hpp"
#include "motor4.hpp"

#define MAX_STEP 50
#define MAX_REFLECTION 10
#define NSAMPLES 1
#define NTHREAD 16


int main(){
    //remove("image.ppm");
    double ratio = 16.0/9.0;
    int width = 1200;
    //int height = (int) width/ratio;

    //Création de la scène
    scene aScene;
    diffuse_material d;
    default_material dd;
    metal me;
    mirror mi;
    glass gl;
    light_material lm;
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


    scene_lights lights;
    ambient_light sky(vec(0,-1,0), color(1,1,1),0);
    point_light l2(point(-2,-0.99,-1),color(1,1,1),0.9);
    point_light l3(point(0,1,-2),color(1,1,1),0.9);
    sphere_light sl(s5, 1);
    lights.add(&sky);
    lights.add(&l2);
    lights.add(&l3);
    lights.add(&sl);

    //Camera
    camera cam(point(0,0.5,1),point(0,0,-1),2.0,ratio);

    //Rendu de l'image
    
    //render(aScene, lights, height, width, cam, sky);
    //motor1 aMotor(aScene, width, ratio, MAX_STEP);
    //motor2 aMotor(aScene, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION);
    //motor3 aMotor(aScene, lights, sky, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION);
    motor4 aMotor(aScene, lights, sky, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION, NTHREAD);
    aMotor.render_image();
    //std::cerr << "\nDone.\n";
    return 0;
}