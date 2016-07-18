#ifndef _RAY_
#define _RAY_

#include "Vect.h"

class Ray{
     Vect ori,dir;
	 
	 public:
	 
	 Ray();
	 
	 Ray(Vect,Vect);
	 
	 Vect getRO() {return ori;}
	 Vect getRD() {return dir;}


};

Ray::Ray(){
	ori=Vect(0,0,0);
    dir=Vect(1,0,0);	
}

Ray::Ray(Vect o, Vect d){
	ori=o;
    dir=d;	
}

#endif