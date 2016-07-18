#ifndef _Sphere_
#define _Sphere_

#include "Vect.h"
#include "Color.h"
#include "math.h"
#include "Object.h"

class Sphere: public Object{
     Vect center;
	 double r;
	 Color color;


	 public:

	 Sphere();

	 Sphere(Vect,double,Color);

	 Vect getSC() {return center;}
	 virtual Color getColor() {return color;}
     double getSr(){return r;}

     virtual    Vect getNormalAt(Vect point){
        //point away from the center
        Vect normal_Vect=point.vectadd(center.negative()).normalize();
        return normal_Vect;
     }

     virtual double findIntersection(Ray ray){
        Vect ray_origin=ray.getRO();
        double ray_origin_x=ray_origin.getVX();
        double ray_origin_y=ray_origin.getVY();
        double ray_origin_z=ray_origin.getVZ();

        Vect ray_direction=ray.getRD();
        double ray_direction_x=ray_direction.getVX();
        double ray_direction_y=ray_direction.getVY();
        double ray_direction_z=ray_direction.getVZ();

        Vect sphere_center=center;
        double sphere_center_x=sphere_center.getVX();
        double sphere_center_y=sphere_center.getVY();
        double sphere_center_z=sphere_center.getVZ();

        double a=1;
        double b=(2*(ray_origin_x-sphere_center_x)*ray_direction_x)+(2*(ray_origin_y-sphere_center_y)*ray_direction_y)+(2*(ray_origin_z-sphere_center_z)*ray_direction_z);
        double c=pow(ray_origin_x-sphere_center_x,2)+pow(ray_origin_y-sphere_center_y,2)+pow(ray_origin_z-sphere_center_z,2)-r*r;

        double dis=b*b-4*a*c;

        if(dis>0){
    //        the ray inserttt;
              double root1=(-b-sqrt(dis))/2 - 0.000001;

              if(root1>0){

                return root1;
              }else{
                double root2=(-b+sqrt(dis))/2 - 0.000001;
                return root2;
              }
        }else{
                //no insert
                return -1;
        }

     }

};

Sphere::Sphere(){
	center=Vect(0,0,0);
	r=1.0;
    color=Color(0.5,0.5,0.5,0);
}

Sphere::Sphere(Vect centerv,double rad, Color c){
	center=centerv;
	r=rad;
    color=c;
}

#endif
