#ifndef _Triangle_
#define _Triangle_

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"
#include "Ray.h"

class Triangle : public Object {
	Vect A,B,C;
	Vect normal;
	double distance;
	Color color;

	public:

	Triangle ();

	Triangle (Vect,Vect,Vect, Color);

	// method functions
	Vect getTriangleNormal () {
	    Vect CA(C.getVX()-A.getVX(),C.getVY()-A.getVY(),C.getVZ()-A.getVZ());
	    Vect BA(B.getVX()-A.getVX(),B.getVY()-A.getVY(),B.getVZ()-A.getVZ());
	    normal=CA.crossProduct(BA).normalize();

	    return normal; }

	double getTriangleDistance () {
	    normal=getTriangleNormal();
	    distance=normal.dotProduct(A);

	    return distance; }


	virtual Color getColor () { return color; }


	virtual Vect getNormalAt(Vect point) {
	    normal=getTriangleNormal();
		return normal;
	}

	virtual double findIntersection(Ray ray) {
		Vect ray_direction = ray.getRD();
        Vect ray_original=ray.getRO();
        normal=getTriangleNormal();
		double a = ray_direction.dotProduct(normal);
        distance=getTriangleDistance();
		if (a == 0) {
			// ray is parallel to the Triangle
			return -1;
		}
		else {
			double b = normal.dotProduct(ray.getRO().vectadd(normal.vectmult(distance).negative()));
			double distance_to_plane=-1*b/a;


			double Qx=ray_direction.vectmult(distance_to_plane).getVX()+ray_original.getVX();
			double Qy=ray_direction.vectmult(distance_to_plane).getVY()+ray_original.getVY();
			double Qz=ray_direction.vectmult(distance_to_plane).getVZ()+ray_original.getVZ();

			Vect Q(Qx,Qy,Qz);

			Vect CA(C.getVX()-A.getVX(),C.getVY()-A.getVY(),C.getVZ()-A.getVZ());
			Vect QA(Q.getVX()-A.getVX(),Q.getVY()-A.getVY(),Q.getVZ()-A.getVZ());
			double test1=CA.crossProduct(QA).dotProduct(normal);

			Vect BC(B.getVX()-C.getVX(),B.getVY()-C.getVY(),B.getVZ()-C.getVZ());
			Vect QC(Q.getVX()-C.getVX(),Q.getVY()-C.getVY(),Q.getVZ()-C.getVZ());
			double test2=BC.crossProduct(QC).dotProduct(normal);

			Vect AB(A.getVX()-B.getVX(),A.getVY()-B.getVY(),A.getVZ()-B.getVZ());
			Vect QB(Q.getVX()-B.getVX(),Q.getVY()-B.getVY(),Q.getVZ()-B.getVZ());
			double test3=AB.crossProduct(QB).dotProduct(normal);

			if(test1>=0 && test2>=0 && test3>=0){
			return -1*b/a;
			}else{
            return -1;
			}
		}
	}

};

Triangle::Triangle () {
    A= Vect(1,0,0);
    B= Vect(0,1,0);
    C= Vect(0,0,1);
	color = Color(0.5,0.5,0.5, 0);
}

Triangle::Triangle (Vect av, Vect bv,Vect cv, Color colorValue) {
	A=av;
	B=bv;
	C=cv;
	color = colorValue;
}

#endif
