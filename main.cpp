#include <fstream>
#include <iostream>
#include <string>
#include "motor1.hpp"
#include "motor2.hpp"
#include "motor3.hpp"
#include "motor4.hpp"
#include "motor5.hpp"
#include "material.hpp"
//#include "description.hpp"

#define MAX_STEP 50
#define MAX_REFLECTION 10
#define NSAMPLES 2
#define NTHREAD 16


int main(){
    double ratio;
    int width;
    scene aScene;
    scene_lights lights;
    ambient_light sky;
    camera cam;
    
    // //Liste des matériaux
    diffuse_material d;
    default_material def;
    metal me;
    mirror mi;
    glass gl;
    light_material lm;
    std::vector<material*> v_material;
    v_material.push_back(&def);
    v_material.push_back(&d);
    v_material.push_back(&me);
    v_material.push_back(&mi);
    v_material.push_back(&gl);
    v_material.push_back(&lm);
    // //Création et ajout des objets dans la scène
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

    // //Création et ajout des lumières dans la scène
     sky = ambient_light(vec(0,-1,0), color(1,1,1),1);
     point_light l2(point(-2,-0.99,-1),color(1,1,1),0.9);
     point_light l3(point(0,1,-2),color(1,1,1),0.9);
     sphere_light sl(s5, 1);
     lights.add(&sky);
     lights.add(&l2);
     lights.add(&l3);
    lights.add(&sl);
    // std::ifstream fichier("scene1.txt", std::ios::in);  // on ouvre le fichier en lecture
    // if(fichier) 
    // {
    //     std::string ligne;
    //     while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
    //     {
    //         std::cout << ligne << std::endl; 
    //         if(ligne == "sphere"){
    //             double value[8];
    //             for(int i=0; i<8;i++){
    //                 fichier >> value[i];
    //             }
    //             sphere s(point(value[0],value[1],value[2]),value[3], color(value[4],value[5],value[6]),v_material[value[7]]);
    //             aScene.add(&s);
    //             }
            
    //         if(ligne == "ground"){
    //             double value[10];
    //             for(int i=0; i<10;i++){
    //                 fichier >> value[i];
    //             }
    //             ground g(point(value[0],value[1],value[2]),vec(value[3],value[4],value[5]),color(value[6],value[7],value[8]),v_material[value[9]]);
    //             aScene.add(&g);
    //         }
    //     }
    //     fichier.close();  
    // }else{
    //     std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    // }
    //Positionnement de la caméra et définition de l'écran
    width = 1200;
    ratio = 16.0/9.0;
    cam = camera(point(0,0.5,1),point(0,0,-1),2.0,ratio);

    //Rendu de l'image
    int motor_index=0;
    while(motor_index<=0 || motor_index>5){
        printf("Quel moteur de rendu voulez-vous utiliser ? : moteur1, moteur2, moteur3, moteur 4, moteur5\n");
        printf("Entrez une valeur entre 1-5: "); 
        scanf("%d", &motor_index);
        if(motor_index==1){
            motor1 aMotor(aScene, width, ratio, MAX_STEP);
            printf("Debut du rendu\n");
            aMotor.render_image();
            printf("Fin du rendu\n");
        }else if(motor_index==2){
            motor2 aMotor(aScene, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION);
            printf("Debut du rendu\n");
            aMotor.render_image();
            printf("Fin du rendu\n");
        }else if(motor_index==3){
            motor3 aMotor(aScene, lights, sky, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION);
            printf("Debut du rendu\n");
            aMotor.render_image();
            printf("Fin du rendu\n");
        }else if(motor_index==4){
            motor4 aMotor(aScene, lights, sky, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION);
            printf("Debut du rendu\n");
            aMotor.render_image();
            printf("Fin du rendu\n");
        }else if(motor_index==5){
            motor5 aMotor(aScene, lights, sky, cam, width, ratio, MAX_STEP, NSAMPLES, MAX_REFLECTION, NTHREAD);
            printf("Debut du rendu\n");
            aMotor.render_image();
            printf("Fin du rendu\n");
        }else{
            printf("Error. Wrong input\n");
        }
    }
    //std::cerr << "\nDone.\n";
    // Error Handling
    return 0;
}