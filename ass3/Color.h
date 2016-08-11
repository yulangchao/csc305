#ifndef _Color_
#define _Color_

class Color{
     double red,green,blue,special;

	 public:

	 Color();

	 Color(double,double,double,double);

	 double getCR() {return red;}
	 double getCG() {return green;}
	 double getCB() {return blue;}
	 double getCS() {return special;}

     double setCR(double redvalue){red=redvalue;}
	 double setCG(double greenvalue){green=greenvalue;}
     double setCB(double bluevalue){blue=bluevalue;}
     double setCS(double specialvalue){special=specialvalue;}


     double brightness(){
         return (red+green+blue)/3;
     }

     Color colorscalar(double scalar){
         return Color(red*scalar, green*scalar,blue*scalar,special);
     }

     Color colorAdd(Color color){
         return Color(red+color.getCR(),green+color.getCG(),blue+color.getCB(),special);

     }

     Color colorMultiply(Color color){
         return Color(red*color.getCR(),green*color.getCG(),blue*color.getCB(),special);

     }
     Color colorAverage(Color color){
         return Color((red+color.getCR())/2,(green+color.getCG())/2,(blue+color.getCB())/2,special);
     }

     	Color clip() {
		double alllight = red + green + blue;
		double excesslight = alllight - 3;
		if (excesslight > 0) {
			red = red + excesslight*(red/alllight);
			green = green + excesslight*(green/alllight);
			blue = blue + excesslight*(blue/alllight);
		}
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}

		return Color (red, green, blue, special);
	}
};

Color::Color(){
	red=0.5;
	green=0.5;
	blue=0.5;

}

Color::Color(double r,double g, double b,double s){
	red=r;
	green=g;
	blue=b;
    special=s	;
}

#endif
