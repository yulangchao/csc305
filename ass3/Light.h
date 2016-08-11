#ifndef _Light_
#define _Light_

#include "Source.h"
#include "Vect.h"
#include "Color.h"

class Light : public Source {


	public:
	Vect position;
	Color color;
	Light ();

	Light (Vect, Color);

	// method functions
	virtual Vect getLightPosition () { return position; }
	virtual Color getLightColor () { return color; }

};

Light::Light () {
	position = Vect(0,0,0);
	color = Color(1,1,1, 0);
}

Light::Light (Vect p, Color c) {
	position = p;
	color = c;
}

#endif
