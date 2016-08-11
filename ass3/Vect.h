#ifndef _VECT_
#define _VECT_

#include "math.h"

class Vect{


	 public:
     double x,y,z;
	 Vect();

	 Vect(double,double,double);

	 double getVX() {return x;}
	 double getVY() {return y;}
	 double getVZ() {return z;}

	 double magnitude(){
		 return sqrt((x*x)+(y*y)+(z*z));

	 }
	 Vect normalize(){

		 double m=sqrt((x*x)+(y*y)+(z*z));
		 return Vect(x/m,y/m,z/m);
	 }

	 Vect negative(){
		 return Vect(-x,-y,-z);

	 }

	 double dotProduct(Vect v){
		 return x*v.getVX()+y*v.getVY()+z*v.getVZ();

	 }
	 Vect crossProduct(Vect v){
		 return Vect (y*v.getVZ()-z*v.getVY(),z*v.getVX()-x*v.getVZ(),x*v.getVY()-y*v.getVX());
	 }
	 Vect vectadd(Vect v){
		 return Vect (x+v.getVX(),y+v.getVY(),z+v.getVZ());
	 }
	Vect vectmult (double scalar) {
		return Vect (x*scalar, y*scalar, z*scalar);
	}
};

Vect::Vect(){
	x=0;
	y=0;
	z=0;
}

Vect::Vect(double i,double j, double k){
	x=i;
	y=j;
	z=k;
}

#endif
