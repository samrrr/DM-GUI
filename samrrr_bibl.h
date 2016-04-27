#include <math.h>			// Header File For Math Library Routines
#include <stdio.h>			// Header File For Standard I/O Routines
#include <stdlib.h>			// Header File For Standard Library

typedef struct{ 
float x,y,z;
}flo3f;

flo3f normvec(flo3f pos1);
void dd(float* x,float* y,float c, float d);
float ss(float x,float y,float x1,float y1);
float ddx(float c, float d);
float ddy(float c, float d);
void roate(float *x,float *y,float x1,float y1,float c);
float ras(float x,float y);
float ricosgr(float plos,float patr);




void dd(float* x,float* y,float c, float d)
{
	float s;
	s=c-90;
	while(!(s < 360)&&!(s >= 0))
	{
		if (s >= 360){s=s-360;};
		if (s < 0) { s=s+360;};
	}
	s=s/180*3.1416926;
	x[0]=cos(s)*d;
	y[0]=sin(s)*d;
}

float ss(float x,float y,float x1,float y1)
{
	float sx,sy;
	float u;
	sx=x;
	sy=y;
	if (sx -x1 == 0) { sx=sx+0.0003;};
	u=atan((sy-y1)/(sx-x1))*180/3.1416926+90;
	if (x1-sx > 0) { u=u+180;};
	u=u-180;
	while(!(u < 360)&&!(u >= 0))
	{
		if (u >= 360){u=u-360;};
		if (u < 0) { u=u+360;};
	}
	return u;
}

float ddx(float c, float d)
{
	float s;
	s=c-90;
	while(!(s < 360)&&!(s >= 0))
	{
		if (s >= 360){s=s-360;};
		if (s < 0) { s=s+360;};
	}
	s=s/180*3.1416926;
	return cos(s)*d;
}

float ddy(float c, float d)
{
	float s;
	s=c-90;
	while(!(s < 360)&&!(s >= 0))
	{
		if (s >= 360){s=s-360;};
		if (s < 0) { s=s+360;};
	}
	s=s/180*3.1416926;
	return sin(s)*d;
}

void roate(float *x,float *y,float x1,float y1,float c)
{
	float u,v;
	u=ss(x1,y1,x[0],y[0]);
	v=ras(x[0]-x1,y[0]-y1);
	x[0]=ddx(u+c,v);
	y[0]=ddy(u+c,v);
	x[0]+=x1;
	y[0]+=y1;
}

float ricosgr(float plos,float patr)
{
	return -(180+patr-plos)+plos;
}

float ras(float x,float y)
{
	return sqrt(x*x+y*y);
}              

float ras3f(float x,float y,float z)
{
	return sqrt(x*x+y*y+z*z);
}              

flo3f normvec(flo3f vec)
{
	//vec/=ras3f(pos1.x,pos1.y,pos1.z);
	float u=ras3f(vec.x,vec.y,vec.z);
	vec.x/=u;
	vec.y/=u;
	vec.z/=u;

	return vec;
}

flo3f getvector(flo3f pos1,flo3f pos2)
{
	flo3f vec;
	//vec=pos2-pos1;
	vec.x=pos2.x-pos1.x;
	vec.y=pos2.y-pos1.y;
	vec.z=pos2.z-pos1.z;
	normvec(vec);

	return vec;
}