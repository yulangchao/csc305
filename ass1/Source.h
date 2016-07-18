#ifndef _SOURCE_
#define _SOURCE_

class Source {
	public:

	Source();

	virtual Vect getLightPosition() {return Vect(0, 0, 0);}
	virtual Color getLightColor() {return Color(1, 1, 1, 0);}

};

Source::Source() {}

#endif
