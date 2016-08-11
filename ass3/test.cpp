#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Source.h"
#include "Triangle.h"

using namespace std;


struct RGBType{
	double r;
	double g;
	double b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data){
	FILE *f;
	int k=w*h;
	int s=4*k;
	int filesize=54+s;

	double factor=39.375;
	int m =static_cast<int>(factor);

	int ppm=dpi*m;

	unsigned char bmpfileheader[14]={'B','M',0,0,0,0,0,0,0,0,54,0,0,0};
	unsigned char bmpinfoheader[40]={40,0,0,0,0,0,0,0,0,0,0,0,1,0,24,0};

	bmpfileheader[2]=(unsigned char)(filesize);
	bmpfileheader[3]=(unsigned char)(filesize>>8);
	bmpfileheader[4]=(unsigned char)(filesize>>16);
	bmpfileheader[5]=(unsigned char)(filesize>>24);

	bmpinfoheader[4]=(unsigned char)(w);
	bmpinfoheader[5]=(unsigned char)(w>>8);
	bmpinfoheader[6]=(unsigned char)(w>>16);
	bmpinfoheader[7]=(unsigned char)(w>>24);

	bmpinfoheader[8]=(unsigned char)(h);
	bmpinfoheader[9]=(unsigned char)(h>>8);
	bmpinfoheader[10]=(unsigned char)(h>>16);
	bmpinfoheader[11]=(unsigned char)(h>>24);

	bmpinfoheader[21]=(unsigned char)(s);
	bmpinfoheader[22]=(unsigned char)(s>>8);
	bmpinfoheader[23]=(unsigned char)(s>>16);
	bmpinfoheader[24]=(unsigned char)(s>>24);

	bmpinfoheader[25]=(unsigned char)(ppm);
	bmpinfoheader[26]=(unsigned char)(ppm>>8);
	bmpinfoheader[27]=(unsigned char)(ppm>>16);
	bmpinfoheader[28]=(unsigned char)(ppm>>24);

	bmpinfoheader[29]=(unsigned char)(ppm);
	bmpinfoheader[30]=(unsigned char)(ppm>>8);
	bmpinfoheader[31]=(unsigned char)(ppm>>16);
	bmpinfoheader[32]=(unsigned char)(ppm>>24);

	f=fopen(filename,"wb");

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for(int i=0;i<k;i++){
		RGBType rgb=data[i];

		double red= (data[i].r)*255;
		double green= (data[i].g)*255;
		double blue= (data[i].b)*255;

		unsigned char color[3]={(int)floor(blue),(int)floor(green),(int)floor(red)};

		fwrite(color,1,3,f);
	}
	fclose(f);
}



int winningObjectIndex(vector<double> object_intersection){
          //return index of winning
          int index_of_minimum_value;

          if(object_intersection.size()==0){
            return -1;
          }
          else if(object_intersection.size()==1){
             if(object_intersection.at(0)>0){
                return 0;
             }else{
                return -1;

             }

          }else{
              //find max
             double max=0;
             for(int i=0;i<object_intersection.size();i++){
                if(max<object_intersection.at(i)){
                    max=object_intersection.at(i);
                }

             }
             if (max>0){
                    for (int j=0;j<object_intersection.size();j++){
                        if(object_intersection.at(j) > 0 && object_intersection.at(j) <= max){
                                    max = object_intersection.at(j);
				                	index_of_minimum_value = j;
                        }
                    }
                    return index_of_minimum_value;
             }else{
                    return -1;
             }
          }

}


Color getColorAt(Vect intersection_position,Vect intersection_ray_direction,vector<Object*> scene_objects,int index_of_winning_object,vector<Source*>light_sources,double accuracy,double ambientlight){
     Color winning_object_color=scene_objects.at(index_of_winning_object)->getColor();
     Vect winning_object_normal=scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);
//floor color
/*
	if (winning_object_color.getCS() == 2) {


		int x = (int)floor(intersection_position.getVX());
		int z = (int)floor(intersection_position.getVZ());

		if (x==z || z+x==0) {

			winning_object_color.setCR(1);
			winning_object_color.setCG(0);
			winning_object_color.setCB(0);
		}
		else if(x==0 ||z==0){
            winning_object_color.setCR(0);
			winning_object_color.setCG(1);
			winning_object_color.setCB(1);

		}else

        {

			winning_object_color.setCR(1);
			winning_object_color.setCG(1);
			winning_object_color.setCR(1);
		}
	}

*/

     Color final_color=winning_object_color.colorscalar(ambientlight);

//reflection of sphere
     	if (winning_object_color.getCS() > 0 && winning_object_color.getCS() <= 1) {
		// reflection from objects with specular intensity
		double dot1 = winning_object_normal.dotProduct(intersection_ray_direction.negative());
		Vect scalar1 = winning_object_normal.vectmult(dot1);
		Vect add1 = scalar1.vectadd(intersection_ray_direction);
		Vect scalar2 = add1.vectmult(2);
		Vect add2 = intersection_ray_direction.negative().vectadd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray (intersection_position, reflection_direction);

		// determine what the ray intersects with first
		vector<double> reflection_intersections;

		for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}

		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1) {
			// reflection ray missed everthing else
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something

				Vect reflection_intersection_position = intersection_position.vectadd(reflection_direction.vectmult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);

				final_color = final_color.colorAdd(reflection_intersection_color.colorscalar(winning_object_color.getCS()));
			}
		}
	}



     //light: shadow

     for(int i=0;i<light_sources.size();i++){
        Vect light_direction=light_sources.at(i)->getLightPosition().vectadd(intersection_position.negative()).normalize();
        float cosine_angle=winning_object_normal.dotProduct(light_direction);
        if(cosine_angle>0){
            //for shadow
            bool shadowed=false;
            Vect distance_to_light=light_sources.at(i)->getLightPosition().vectadd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude= distance_to_light.magnitude();

            Ray shadow_ray(intersection_position,light_sources.at(i)->getLightPosition().vectadd(intersection_position.negative()).normalize());

            vector<double> secondary_intersections;

            for(int k=0;k<scene_objects.size()&& shadowed==false;k++){
                secondary_intersections.push_back(scene_objects.at(k)->findIntersection(shadow_ray));
            }

            for(int c=0;c<secondary_intersections.size();c++){
                if(secondary_intersections.at(c)>accuracy){

                     if(secondary_intersections.at(c)<=distance_to_light_magnitude){

                        shadowed=true;
                     }
                     break;
                }

            }

            if(shadowed==false){
                final_color=final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(i)->getLightColor()).colorscalar(cosine_angle));

                if(winning_object_color.getCS()>0 && winning_object_color.getCS()<=1){
                    double dot1=winning_object_normal.dotProduct(intersection_ray_direction.negative());
                    Vect scalar1=winning_object_normal.vectmult(dot1);
                    Vect add1=scalar1.vectadd(intersection_ray_direction);
                    Vect scalar2=add1.vectmult(2);
                    Vect add2=intersection_ray_direction.negative().vectadd(scalar2);
                    Vect reflection_direction=add2.normalize();
                    double specular=reflection_direction.dotProduct(light_direction);

                    if(specular>0){

                        specular=pow(specular,10);
                        final_color=(final_color.colorAdd(light_sources.at(i)->getLightColor().colorscalar(specular*winning_object_color.getCS())));

                    }
                }
                 if(winning_object_color.getCS()==2){
                    double dot1=winning_object_normal.dotProduct(intersection_ray_direction.negative());
                    Vect scalar1=winning_object_normal.vectmult(dot1);
                    Vect add1=scalar1.vectadd(intersection_ray_direction);
                    Vect scalar2=add1.vectmult(2);
                    Vect add2=intersection_ray_direction.negative().vectadd(scalar2);
                    Vect reflection_direction=add2.normalize();
                    double specular=reflection_direction.dotProduct(light_direction);

                    if(specular>0){

                        specular=pow(specular,10);
                        final_color=(final_color.colorAdd(light_sources.at(i)->getLightColor().colorscalar(specular*winning_object_color.getCS())));

                    }
                }
            }
        }
     }

     return final_color.clip();

}



Vect moving(Vect center,double x1, double y1,double z1){
    double m[4][4]=
    {
        {1,0,0,x1},
        {0,1,0,y1},
        {0,0,1,z1},
        {0,0,0,1},
    };
   // printf("%f",(center.getVX()));
    center=Vect(center.x*m[0][0]+center.y*m[0][1]+center.z*m[0][2]+1*m[0][3],center.x*m[1][0]+center.y*m[1][1]+center.z*m[1][2]+1*m[1][3],center.x*m[2][0]+center.y*m[2][1]+center.z*m[2][2]+1*m[2][3]);
    printf("%f,%f,%f",center.x,center.y,center.z);
    return center;

}
int thisone;


int main(int argc, char *argv[]){
     cout<<"rendering"<<endl;



	 int dpi=72;
	 int width=640;
	 int height=480;
	 double aspect=(double)(width)/(double)height;
	 int n=width*height;
	 RGBType *pixels= new RGBType[n];

     int aadepth=1;
     double aathreshold=0.1;

	 double aspectratio=(double)width/(double)height;
	 double ambientlight=0.2;
	 double accuracy=0.000001;

	 Vect X (1,0,0);
	 Vect Y (0,1,0);
	 Vect Z (0,0,1);
	 Vect O (0,0,0);
	 Vect cpos(0,3,-10);
	 Vect init(0,3,0);
	 Vect vd (cpos.getVX()-init.getVX(),cpos.getVY()-init.getVY(),cpos.getVZ()-init.getVZ());

	 Vect cdir=vd.negative().normalize();
	 Vect cright=Y.crossProduct(cdir).normalize();
	 Vect cdown=	cright.crossProduct(cdir);
	 Camera cam (cpos,cdir,cright,cdown);



	 //color
	 Color w_Light(0.05,0.05,0.05,0);      //white light
	 Color green(0.5,1,0.5,0.1);
	 Color gray(0.5,0.5,0.5,0);
	 Color black(0,0,0,0);
	 Color skyBlue(0.6, 0.6, 0.9, 0.4);
     Color silver(0.75, 0.75, 0.75, 0);
     Color white1(1,1,1,1.5);
     Color Red(1,0.5,0.5,0);
     Color green1(0.5,1,0.5,0);
     Color b(1,0.5,0.5,0);
     Color white2(100,100,100,0);


	vector<Source*> light_sources;
    Vect lightpos[5][5];
    Light scene_light[5][5];
     for(int m=0;m<5;m++){
        for(int n=0;n<5;n++){
            lightpos[m][n]= Vect((m-2.5)/4,6.98,(n-2.5)/4);
            scene_light[m][n]= Light(lightpos[m][n],w_Light );
            light_sources.push_back(dynamic_cast<Source*>(&scene_light[m][n]));
        }
     }

	 double x1,y1;
	 //shpere
	 Sphere sph (Vect(0.0,0,0.0),1,skyBlue);

	 Sphere sph1 (moving(O,0,1.5,0),0.5,silver);
	 Sphere sph2 (Vect(0.5,0,7),1,green);
	// Sphere sph2 (O,1,skyBlue);
//	 Sphere sph (Y,1,green)
	Plane splane (Y, -1, gray);
/*	Plane splane1 (Y, 7, black);
Plane splane2 (X, -4, green1    );
Plane splane3 (X, 4, Red);
Plane splane4 (Z, -4, gray);
Plane splane5 (Z, 4, gray);
Vect AA(0.5,6.9,0.5);
Vect BB(0.5,6.9,-0.5);
Vect CC(-0.5,6.9,-0.5);
Vect DD(-0.5,6.9,0.5);
Triangle triA(CC,AA,BB,w_Light);
Triangle triB(DD,AA,CC,w_Light);

*/
	Vect A(1,-1,-1);
    Vect B(1,-1,-2);
	Vect C(1,0,-2);
    Vect D(1,0,-1);
	Vect E(0,-1,-1);
    Vect F(0,-1,-2);
	Vect G(0,0,-2);
    Vect H(0,0,-1);


    Vect A1(4,-1,4);
    Vect B1(4,-1,-7);
	Vect C1(4,7,-7);
    Vect D1(4,7,4);
	Vect E1(-4,-1,4);
    Vect F1(-4,-1,-7);
	Vect G1(-4,7,-7);
    Vect H1(-4,7,4);

	Vect C2(2.5/4,6.9999,-2.5/4);
    Vect D2(2.5/4,6.9999,2.5/4);
	Vect G2(-2.5/4,6.9999,-2.5/4);
    Vect H2(-2.5/4,6.9999,2.5/4);

    A.x-=3;
    B.x-=3;
    C.x-=3;
    D.x-=3;
    E.x-=3;
    F.x-=3;
    G.x-=3;
    H.x-=3;


	Triangle tri(A,C,B,b);
	Triangle tri1(A,D,C,b);
	Triangle tri2(A,E,D,b);
    Triangle tri3(E,H,D,b);

	Triangle tri4(A,B,E,b);
	Triangle tri5(F,E,B,b);
	Triangle tri6(B,C,G,b);
    Triangle tri7(G,F,B,b);

    Triangle tri8(H,G,C,b);
	Triangle tri9(C,D,H,b);
	Triangle tri10(G,H,F,b);
    Triangle tri11(E,F,H,b);

    Triangle atri(A1,B1,C1,Red);
	Triangle atri1(A1,C1,D1,Red);
	Triangle atri2(A1,D1,E1,gray);
    Triangle atri3(H1,E1,D1,gray);

	Triangle atri4(A1,E1,B1,gray);
	Triangle atri5(F1,B1,E1,gray);
	//Triangle atri6(G1,B1,C1,b);
    //Triangle atri7(G1,F1,dB1,b);

    Triangle atri8(H1,C1,G1,white1);
	Triangle atri9(C1,H1,D1,white1);
	Triangle atri10(G1,F1,H1,green1);
    Triangle atri11(E1,H1,F1,green1);

    Triangle L8(C2,H2,G2,white2);
	Triangle L9(H2,C2,D2,white2);
	 vector<Object*> scene_objects;
  //   scene_objects.push_back(dynamic_cast<Object*>(&triA));
//	 scene_objects.push_back(dynamic_cast<Object*>(&triB));
	 scene_objects.push_back(dynamic_cast<Object*>(&sph));
     scene_objects.push_back(dynamic_cast<Object*>(&sph1));
     scene_objects.push_back(dynamic_cast<Object*>(&sph2));

	 scene_objects.push_back(dynamic_cast<Object*>(&splane));
     //scene_objects.push_back(dynamic_cast<Object*>(&splane1));
     //	 scene_objects.push_back(dynamic_cast<Object*>(&splane2));
    // scene_objects.push_back(dynamic_cast<Object*>(&splane3));
    // 	 scene_objects.push_back(dynamic_cast<Object*>(&splane4));
  //   scene_objects.push_back(dynamic_cast<Object*>(&splane5));
     scene_objects.push_back(dynamic_cast<Object*>(&tri));
	 scene_objects.push_back(dynamic_cast<Object*>(&tri1));
	 scene_objects.push_back(dynamic_cast<Object*>(&tri2));
     scene_objects.push_back(dynamic_cast<Object*>(&tri3));
     scene_objects.push_back(dynamic_cast<Object*>(&tri4));
     scene_objects.push_back(dynamic_cast<Object*>(&tri5));
	 scene_objects.push_back(dynamic_cast<Object*>(&tri6));
	 scene_objects.push_back(dynamic_cast<Object*>(&tri7));
	 scene_objects.push_back(dynamic_cast<Object*>(&tri8));
     scene_objects.push_back(dynamic_cast<Object*>(&tri9));
     scene_objects.push_back(dynamic_cast<Object*>(&tri10));
     scene_objects.push_back(dynamic_cast<Object*>(&tri11));
     //big box
     scene_objects.push_back(dynamic_cast<Object*>(&atri));
	 scene_objects.push_back(dynamic_cast<Object*>(&atri1));
	 scene_objects.push_back(dynamic_cast<Object*>(&atri2));
     scene_objects.push_back(dynamic_cast<Object*>(&atri3));
     scene_objects.push_back(dynamic_cast<Object*>(&atri4));
     scene_objects.push_back(dynamic_cast<Object*>(&atri5));
//	 scene_objects.push_back(dynamic_cast<Object*>(&atri6));
//	 scene_objects.push_back(dynamic_cast<Object*>(&atri7));
	 scene_objects.push_back(dynamic_cast<Object*>(&atri8));
     scene_objects.push_back(dynamic_cast<Object*>(&atri9));
     scene_objects.push_back(dynamic_cast<Object*>(&atri10));
     scene_objects.push_back(dynamic_cast<Object*>(&atri11));
	 scene_objects.push_back(dynamic_cast<Object*>(&L8));
     scene_objects.push_back(dynamic_cast<Object*>(&L9));
	int thisone, aa_index;
	double xamnt, yamnt;
	double tempRed, tempGreen, tempBlue;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			thisone = y*width + x;

			// start with a blank pixel
			double tempRed[aadepth*aadepth];
			double tempGreen[aadepth*aadepth];
			double tempBlue[aadepth*aadepth];

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(0));

					// create the ray from the camera to this pixel
					if (aadepth == 1) {

						// start with no anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = ((height - y) + 0.5)/height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + 0.5)/ width;
							yamnt = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + 0.5)/width;
							yamnt = ((height - y) + 0.5)/height;
						}
					}
					else {
						// anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
							yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + (double)aax/((double)aadepth - 1))/ width;
							yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else {
							// the image is square
							xamnt = (x + (double)aax/((double)aadepth - 1))/width;
							yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
						}
					}

					Vect cam_ray_origin = cam.getCP();
					Vect cam_ray_direction = cdir.vectadd(cright.vectmult(xamnt - 0.5).vectadd(cdown.vectmult(yamnt - 0.5))).normalize();

					Ray cam_ray (cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (int index = 0; index < scene_objects.size(); index++) {
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
					}

					int index_of_winning_object = winningObjectIndex(intersections);

					if (index_of_winning_object == -1) {
						// set the backgroung black
						tempRed[aa_index] = 0;
						tempGreen[aa_index] = 0;
						tempBlue[aa_index] = 0;
					}
					else{
						// index coresponds to an object in our scene
						if (intersections.at(index_of_winning_object) > accuracy) {
							// determine the position and direction vectors at the point of intersection

							Vect intersection_position = cam_ray_origin.vectadd(cam_ray_direction.vectmult(intersections.at(index_of_winning_object)));
							Vect intersecting_ray_direction = cam_ray_direction;

							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);

							tempRed[aa_index] = intersection_color.getCR();
							tempGreen[aa_index] = intersection_color.getCG();
							tempBlue[aa_index] = intersection_color.getCB();
						}
					}
				}
			}

			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed/(aadepth*aadepth);
			double avgGreen = totalGreen/(aadepth*aadepth);
			double avgBlue = totalBlue/(aadepth*aadepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
		}
	}
	 savebmp("s1.bmp",width,height,dpi,pixels);
	 	delete pixels, tempRed, tempGreen, tempBlue;
	 return 0;
}
