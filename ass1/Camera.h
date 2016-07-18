#ifndef _Camera_
#define _Camera_

#include "Vect.h"

class Camera{
     Vect cpos,cdir,cright,cdown;

	 public:

	 Camera();

	 Camera(Vect,Vect,Vect,Vect);

	 Vect getCP() {return cpos;}
	 Vect getCD() {return cdir;}
	 Vect getCR() {return cright;}
	 Vect getCDown() {return cdown;}


};

Camera::Camera(){
	cpos=Vect(0,0,0);
    cdir=Vect(0,0,1);
    cright=Vect(0,0,0);
    cdown=Vect(0,0,0);
}

Camera::Camera(Vect pos, Vect dir,Vect right, Vect down){
	cpos=pos;
    cdir=dir;
    cright=right;
    cdown=down;
}

#endif
