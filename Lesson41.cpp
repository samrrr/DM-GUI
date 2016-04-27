#include <windows.h>		
#include <math.h>		
#include <stdio.h>		
#include <stdlib.h>		
#include <conio.h>  
#include <string.h> 
#include <time.h>    
#include <gl\gl.h>  
#include <gl\glu.h> 
#include <olectl.h>
#include <stdarg.h>  
#include <mmsystem.h>

#include "samrrr_bibl.h"
#include "NeHeGL.h"		
#include "megalib.h"

#pragma comment( lib, "opengl32.lib" )								
#pragma comment( lib, "glu32.lib" )									

static BOOL g_isProgramLooping;				
static BOOL g_createFullScreen;
#ifndef CDS_FULLSCREEN												
#define CDS_FULLSCREEN 4												
#endif																
int exitgame
, razokx =/*/640/*/GetSystemMetrics(SM_CXSCREEN)/**/
, razoky =/*/480/*/GetSystemMetrics(SM_CYSCREEN)/**/;
float fps = 0;

GL_Window			window;

GLuint  base;      // База списка отображения для фонта

HDC HDc;




//#define MAX_AST 30
#define WM_TOGGLEFULLSCREEN (WM_USER+1)								

typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;													// Structure Name
TextureImage textureTGA[60];

int mx, my, mxz, myz, mch, mchz, mchl, mchr, mchlz, mchrz;

GL_Window*	g_window;													// Window Structure
Keys*		g_keys;														// Keyboard



struct                    // Создание структуры для информации о таймере
{
	__int64       frequency;                 // Частота таймера
	float         resolution;                // Точность таймера
	unsigned long mm_timer_start;            // Стартовое значение мультимедийного таймера
	unsigned long mm_timer_elapsed;          // Прошедшее время мультимедийного таймера
	bool          performance_timer;         // Использовать высокоточный таймер?
	__int64       performance_timer_start;   // Стартовое значение высокоточного таймера
	__int64       performance_timer_elapsed; // Прошедшее время высокоточного таймера
} mytimer;

typedef struct
{
	float c1, c2, c3, x, y, z;
} ggcam;
ggcam cam;

/*
typedef struct
{
	float movup, movleft, movroat, x, y, z, xx, yy, zz, xm, ym, zm, mass, c1, c2, c3, c3z, c3z1;
	int t, needremax, minx, maxx, miny, maxy, minz, maxz;
	ggblock block[MAX_SIZE_AST][MAX_SIZE_AST][MAX_SIZE_AST];
} ggast;
ggast ast[MAX_AST];
*/

TextureImage basetex[100];

int input_state = 0;//0-menu 1-count
int input_type = 0;//0-a+b,1-a-b,2-a*b

typedef struct{
	union
	{
		struct{ float x, y, z; };
		float f[3];
	};
}Vec;

long ti;
int font_x;

GLuint	texture[10];

float fpsla[10];

void exit_prigram(){ exitgame = 1; }
void circl(float x, float y, float z, float c);
void movveforw(float cam_c, float cam_c1, float cam_c2, float rass, float *cam_x, float *cam_y, float *cam_z);
void movve(float *cam_c, float *cam_c1, float *cam_c2, float x, float y);
void polyp(int _x0, int  _y0, int  _x1, int  _y1, int _x2, int  _y2, int  _x3, int  _y3, float _r, float _g, float _b, float _alpha);
void count_res(int,int,int);
void count_res_draw();
GLvoid glPrint(const char *fmt, ...);


#include <string>

using namespace std;

class INP
{
public:
	string s;
	string s_b;
	int selected;
	int x0, y0, x1, y1;

	INP(int _x0, int _y0, int _x1, int _y1, char*_s)
	{
		s_b = _s;
		selected = 0;
		s = "";
		x0 = _x0;
		y0 = _y0;
		x1 = _x1;
		y1 = _y1;
	}

	void addsymbol(char ch)
	{
		if (selected == 0)
			return;
		if (ch != '\b')
		{
			s = s + ch;
		}
		else
		{
			if (s.size()>0)
				s.resize(s.size() - 1);
		}
	}
	void draw()
	{
		polyp(
			x0 - 4, y0 - 4,
			x0 - 4, y1 + 4,
			x1 + 4, y1 + 4,
			x1 + 4, y0 - 4,
			0.4, 0.4, 1, 1);

		if (selected)
			polyp(
			x0 - 4, y0 - 4,
			x0 - 4, y1 + 4,
			x1 + 4, y1 + 4,
			x1 + 4, y0 - 4,
			1.0, 0.5, 0.5, 1);

		polyp(
			x0, y0,
			x0, y1,
			x1, y1,
			x1, y0,
			1, 1, 1, 1);


		glColor4f(0.1, 0.1, 0.1, 1);
		glRasterPos3f((x0+10 - razokx / 2) * 2 / (float)razoky, (razoky / 2 - y0-22) * 2 / (float)razoky, -1);
		glPrint((s_b+s).data());

	}
};

INP inpu1(10, razoky / 2 - 50, razokx - 10, razoky / 2 - 20, "a:");
INP inpu2(10, razoky / 2 - 10, razokx - 10, razoky / 2 + 20, "b:");
INP inpu3(10, razoky / 2 + 30, razokx - 10, razoky / 2 + 60, "c:");
INP inpu4(10, razoky / 2 + 70, razokx - 10, razoky / 2 + 100, "d:");
INP inpu7(razokx / 2 - 80, razoky / 2 + 110, razokx / 2 + 80, razoky / 2 + 140, "CALCULATE");
INP inpu5(10, razoky / 2 + 150, razokx - 10, razoky / 2 + 180, "RES:");
INP inpu6(razokx / 2 - 50, razoky / 2 + 190, razokx / 2 + 50, razoky / 2 + 220, "Back");
string cou_title;

struct BUTTON{
	int x0, y0, x1, y1;//0..60
	void(*fu)(int _x0, int  _y0, int  _x1, int  _y1, float _alpha);
	void(*click)();
	float last_press;
	float last_mouse;
	float time_mouse;
	int zav;
};

void addi(int&i, int add)
{
	i += add;
	if (i < 0)
		i = 0;
	if (i > 100000)
		i = 100000;
}

double s_a(double a, double b, double c)
{
	return(a * c + b * (1 - c));
}

class GUI
{
private:
	int n;
	void(*fun[100])();
	BUTTON bu[100];
	int in_button;
	
	float zoom;
	float  px, py;
	float scale_x;
	
	float old_zoom;
	float old_px, old_py;
	float old_scale_x;
	
	int scx, scy;

	int press;
	int press_to;
	int in_button_to;
	int out_button_to;
	int out_button;
public:
	GUI(int _scx,int _scy)
	{
		scx = _scx;
		scy = _scy;
		n = 0;
		in_button = -1;
		in_button_to = 0;
		press = 0;
	}


	int addbutton(int _x0, int  _y0, int  _x1, int  _y1, void(*_fu)(int _x0, int  _y0, int  _x1, int  _y1, float _alpha), void(*_click)(), int _zav)
	{
		bu[n].x0 = _x0;
		bu[n].y0 = _y0;
		bu[n].x1 = _x1;
		bu[n].y1 = _y1;
		bu[n].fu = _fu;
		bu[n].click = _click;
		bu[n].last_mouse = 99999;
		bu[n].last_press = 99999;
		bu[n].time_mouse = 0;
		bu[n].zav = _zav;
		n++;

		return n - 1;
	}


	void upd(int mx,int my,int mch,int mis)
	{
		int i, r, o, j;
//		mis /= 10;
		press_to -= mis;
		if (press_to < 0)
			press_to = 0;

		in_button_to -= mis;
		if (in_button_to < 0)
		{
			in_button_to = 0;
			if (bu[in_button].click != NULL)
			{
				bu[in_button].click();

				int x[5], y[5];
				i = in_button;

				in_button = bu[i].zav;
				out_button_to = 1000;
				r = i;

				out_button = i;

				y[0] = bu[r].y1 - bu[r].y0;
				x[0] = bu[r].x1 - bu[r].x0;

				y[4] = (bu[r].y0) * 60 / y[0];
				x[4] = (bu[r].x0) * 60 / x[0];

				old_scale_x = x[0] / (float)y[0];
				old_zoom = 60 / (float)y[0];
				old_px = x[4] / old_zoom;
				old_py = y[4] / old_zoom;
			}
		}

		addi(out_button_to, -mis);

		scale_x = 1;
		zoom = 1;
		px = 0;
		py = 0;


		if (press == 0)
			for (i = 0; i < n; i++)
				if (bu[i].zav == in_button)
				if (mch == 1)
				{
					int x0, y0, x1, y1;

					x0 = bu[i].x0*scx / 60;
					y0 = bu[i].y0*scy / 60;
					x1 = bu[i].x1*scx / 60;
					y1 = bu[i].y1*scy / 60;
					if (mx >= x0 && my >= y0 && mx < x1 && my < y1)
					{
						mch = 0;

						if (bu[i].click == NULL)
						{
							int x[5], y[5];

							in_button = i;
							in_button_to = 1000;

							y[0] = bu[i].y1 - bu[i].y0;
							x[0] = bu[i].x1 - bu[i].x0;


							y[4] = (bu[i].y0) * 60 / y[0];
							x[4] = (bu[i].x0) * 60 / x[0];

							old_scale_x = x[0] / (float)y[0];
							old_zoom = 60 / (float)y[0];
							old_px = x[4] / old_zoom;
							old_py = y[4] / old_zoom;
						}
						else
							if (bu[i].click == (void(*)())exit)
							{
								if (bu[i].zav != -1)
								{
									int x[5], y[5];

									in_button = bu[bu[i].zav].zav;
									out_button_to = 1000;
									r = bu[i].zav;

									out_button = bu[i].zav;

									y[0] = bu[r].y1 - bu[r].y0;
									x[0] = bu[r].x1 - bu[r].x0;

									y[4] = (bu[r].y0) * 60 / y[0];
									x[4] = (bu[r].x0) * 60 / x[0];

									old_scale_x = x[0] / (float)y[0];
									old_zoom = 60 / (float)y[0];
									old_px = x[4] / old_zoom;
									old_py = y[4] / old_zoom;

								}
								else
								{
									exit_prigram();
								}
									
							}
							else
							{
								int x[5], y[5];
								in_button = i;
								in_button_to = 1000;

								y[0] = bu[i].y1 - bu[i].y0;
								x[0] = bu[i].x1 - bu[i].x0;


								y[4] = (bu[i].y0) * 60 / y[0];
								x[4] = (bu[i].x0) * 60 / x[0];

								old_scale_x = x[0] / (float)y[0];
								old_zoom = 60 / (float)y[0];
								old_px = x[4] / old_zoom;
								old_py = y[4] / old_zoom;
							}
					}
				}
		if (in_button_to > 0)
		{
			zoom = s_a(1, old_zoom, 1 - in_button_to / 1000.0);
			scale_x = s_a(1, old_scale_x, 1 - in_button_to / 1000.0);
			px = s_a(0, old_px, 1 - in_button_to / 1000.0);
			py = s_a(0, old_py, 1 - in_button_to / 1000.0);
		}
		if (out_button_to > 0)
		{
			zoom = s_a(1, old_zoom, out_button_to / 1000.0);
			scale_x = s_a(1, old_scale_x, out_button_to / 1000.0);
			px = s_a(0, old_px, out_button_to / 1000.0);
			py = s_a(0, old_py, out_button_to / 1000.0);
		}

	}
	void resize(int _scx, int _scy)
	{
		scx = _scx;
		scy = _scy;
	}

	void draw()
	{
		int i, r, o, j;
		if (out_button_to == 0)
		{
			for (i = 0; i < n; i++)
				if (bu[i].zav == in_button)
				{
					int x0, y0, x1, y1;
					float a;

					a = 1;
					x0 = (bu[i].x0 / zoom + px)*scale_x*scx / 60 + 3;
					y0 = (bu[i].y0 / zoom + py)*scy / 60 + 3;
					x1 = (bu[i].x1 / zoom + px)*scale_x*scx / 60 - 3;
					y1 = (bu[i].y1 / zoom + py)*scy / 60 - 3;


					bu[i].fu(x0, y0, x1, y1, a);
				}
			if (in_button != -1)
				if (in_button_to > 0)
					for (i = 0; i < n; i++)
						if (bu[i].zav == bu[in_button].zav)
						{
							int x0, y0, x1, y1;
							float a;

							a = 1;

							if (i == in_button)
							{
								a = in_button_to / 1000.0;
							}

							float x[5], y[5];

							x[0] = bu[in_button].x0;
							y[0] = bu[in_button].y0;
							x[1] = bu[in_button].x1;
							y[1] = bu[in_button].y1;

							x[2] = (bu[i].x0 - x[0]) * old_zoom / old_scale_x;
							y[2] = (bu[i].y0 - y[0]) * old_zoom;
							x[3] = (bu[i].x1 - x[0]) * old_zoom / old_scale_x;
							y[3] = (bu[i].y1 - y[0]) * old_zoom;

							x0 = (x[2] / zoom + px) *scale_x *scx / 60 + 3;
							y0 = (y[2] / zoom + py) *scy / 60 + 3;
							x1 = (x[3] / zoom + px) *scale_x *scx / 60 - 3;
							y1 = (y[3] / zoom + py) *scy / 60 - 3;


							bu[i].fu(x0, y0, x1, y1, a);
						}
		}
		else
		{
			for (i = 0; i < n; i++)
				if (bu[i].zav == out_button)
				{
					int x0, y0, x1, y1;
					float a;

					a = 1;
					x0 = (bu[i].x0 / zoom + px)*scale_x*scx / 60 + 3;
					y0 = (bu[i].y0 / zoom + py)*scy / 60 + 3;
					x1 = (bu[i].x1 / zoom + px)*scale_x*scx / 60 - 3;
					y1 = (bu[i].y1 / zoom + py)*scy / 60 - 3;


					bu[i].fu(x0, y0, x1, y1, a);
				}
				if (out_button_to > 0)
					for (i = 0; i < n; i++)
						if (bu[i].zav == in_button)
						{
							int x0, y0, x1, y1;
							float a;

							a = 1;

							if (i == out_button)
							{
								a = 1 - out_button_to / 1000.0;
							}

							float x[5], y[5];

							x[0] = bu[out_button].x0;
							y[0] = bu[out_button].y0;
							x[1] = bu[out_button].x1;
							y[1] = bu[out_button].y1;

							x[2] = (bu[i].x0 - x[0]) * old_zoom / old_scale_x;
							y[2] = (bu[i].y0 - y[0]) * old_zoom;
							x[3] = (bu[i].x1 - x[0]) * old_zoom / old_scale_x;
							y[3] = (bu[i].y1 - y[0]) * old_zoom;

							x0 = (x[2] / zoom + px) *scale_x *scx / 60 + 3;
							y0 = (y[2] / zoom + py) *scy / 60 + 3;
							x1 = (x[3] / zoom + px) *scale_x *scx / 60 - 3;
							y1 = (y[3] / zoom + py) *scy / 60 - 3;


							bu[i].fu(x0, y0, x1, y1, a);
						}
		}
	}
};

GUI gui(razokx,razoky);



void rect(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float x0, y0, x1, y1;

	x0 = (-razokx + _x0 * 2) / (float)razoky;
	y0 = (razoky - _y0 * 2) / (float)razoky;
	x1 = (-razokx + _x1 * 2) / (float)razoky;
	y1 = (razoky - _y1 * 2) / (float)razoky;

	glColor4f(0.4, 0.4, 1, _alpha);

	glBegin(GL_POLYGON);
	glVertex3f(x0, y0, -1);
	glVertex3f(x0, y1, -1);
	glVertex3f(x1, y1, -1);
	glVertex3f(x1, y0, -1);
	glEnd();
}
void polyp(int _x0, int  _y0, int  _x1, int  _y1, int _x2, int  _y2, int  _x3, int  _y3, float _r, float _g, float _b, float _alpha)
{
	float x0, y0, x1, y1, x2, y2, x3, y3;

	x0 = (-razokx + _x0 * 2) / (float)razoky;
	y0 = (razoky - _y0 * 2) / (float)razoky;
	x1 = (-razokx + _x1 * 2) / (float)razoky;
	y1 = (razoky - _y1 * 2) / (float)razoky;
	x2 = (-razokx + _x2 * 2) / (float)razoky;
	y2 = (razoky - _y2 * 2) / (float)razoky;
	x3 = (-razokx + _x3 * 2) / (float)razoky;
	y3 = (razoky - _y3 * 2) / (float)razoky;

	glColor4f(_r,_g,_b, _alpha);

	glBegin(GL_POLYGON);
	glVertex3f(x0, y0, -1);
	glVertex3f(x1, y1, -1);
	glVertex3f(x2, y2, -1);
	glVertex3f(x3, y3, -1);
	glEnd();
}


void put_N(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0+(_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0+(_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (float)(x1 - x0), y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 0 * (float)(x1 - x0), y0 + 1 * (float)(x1 - x0),
		1, 1, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 / 2.0 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 1 / 2.0 * (float)(x1 - x0),
		1, 1, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (float)(x1 - x0), y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (float)(x1 - x0), y0 + 1 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		1, 1, 1, _alpha);

	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 / 2.0 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 1 / 2.0 * (float)(x1 - x0),
		0.4, 0.4, 1, _alpha);

}


void put_I(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;


	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);

}


void put_R(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (float)(x1 - x0), y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 0 * (float)(x1 - x0), y0 + 1 * (float)(x1 - x0),
		1, 1, 1, _alpha);

	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);

}


void put_P(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (float)(x1 - x0), y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (float)(x1 - x0),
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (float)(x1 - x0),
		x0 + 0 * (float)(x1 - x0), y0 + 1 * (float)(x1 - x0),
		1, 1, 1, _alpha);

	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);

}


void put_O(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 0.4, 0.4, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 1, 0.4, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.8, 0.8, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.8, 0.8, 0.4, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.8, 0.4, 0.8, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1.0, 0.4, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 1.0, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1.0, 1.0, 0.4, _alpha);


}


void put_back(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);

	polyp(
		x0 + 1 / 4.0 * (float)(x1 - x0), y0 + 0 / 4.0 * (float)(x1 - x0),
		x0 + 4 / 4.0 * (float)(x1 - x0), y0 + 3 / 4.0 * (float)(x1 - x0),
		x0 + 3 / 4.0 * (float)(x1 - x0), y0 + 4 / 4.0 * (float)(x1 - x0),
		x0 + 0 / 4.0 * (float)(x1 - x0), y0 + 1 / 4.0 * (float)(x1 - x0),
		0.7, 1, 0.7, _alpha);
	polyp(
		x0 + 3 / 4.0 * (float)(x1 - x0), y0 + 0 / 4.0 * (float)(x1 - x0),
		x0 + 4 / 4.0 * (float)(x1 - x0), y0 + 1 / 4.0 * (float)(x1 - x0),
		x0 + 1 / 4.0 * (float)(x1 - x0), y0 + 4 / 4.0 * (float)(x1 - x0),
		x0 + 0 / 4.0 * (float)(x1 - x0), y0 + 3 / 4.0 * (float)(x1 - x0),
		0.7, 1, 0.7, _alpha);

}


void put_EX(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);

	polyp(
		x0 + 1 / 4.0 * (float)(x1 - x0), y0 + 0 / 4.0 * (float)(x1 - x0),
		x0 + 4 / 4.0 * (float)(x1 - x0), y0 + 3 / 4.0 * (float)(x1 - x0),
		x0 + 3 / 4.0 * (float)(x1 - x0), y0 + 4 / 4.0 * (float)(x1 - x0),
		x0 + 0 / 4.0 * (float)(x1 - x0), y0 + 1 / 4.0 * (float)(x1 - x0),
		1, 0.5, 0.5, _alpha);
	polyp(
		x0 + 3 / 4.0 * (float)(x1 - x0), y0 + 0 / 4.0 * (float)(x1 - x0),
		x0 + 4 / 4.0 * (float)(x1 - x0), y0 + 1 / 4.0 * (float)(x1 - x0),
		x0 + 1 / 4.0 * (float)(x1 - x0), y0 + 4 / 4.0 * (float)(x1 - x0),
		x0 + 0 / 4.0 * (float)(x1 - x0), y0 + 3 / 4.0 * (float)(x1 - x0),
		1, 0.5, 0.5, _alpha);
}


void put_plu(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}


void put_min(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}


void put_mno(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}


void put_del(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}


void put_mod(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}



void put_ste(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (xx1 - xx0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		1, 1, 1, _alpha);

	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 0 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 0 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 0 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 2 * (x1 - x0) / 3, y0 + 1 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
	polyp(
		x0 + 1 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 2 * (x1 - x0) / 3,
		x0 + 3 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		x0 + 1 * (x1 - x0) / 3, y0 + 3 * (x1 - x0) / 3,
		0.4, 0.4, 1.0, _alpha);
}



void put_ste_mod(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 2;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (yy1 - yy0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	int ma[] =
	{
		5, 0,
		10, 10,
		10, 20,
		5, 10,
		1,

		5, 0,
		5, 10,
		0, 20,
		0, 10,
		1,

		25, 0,
		30, 10,
		20, 30,
		15, 20,
		1,

		15, 0,
		20, 0,
		20, 10,
		15, 10,
		1,

		25, 20,
		30, 20,
		30, 30,
		25, 30,
		1,

		15, 10,
		15, 10,
		20, 10,
		15, 20,
		0,

		20, 0,
		20, 0,
		25, 0,
		20, 10,
		0,

		25, 0,
		25, 0,
		30, 0,
		30, 10,
		0,

		30, 10,
		30, 10,
		30, 20,
		25, 20,
		0,

		25, 20,
		25, 20,
		25, 30,
		20, 30,
		0,

		0, 0,
		0, 0,
		5, 0,
		0, 10,
		0,

		5, 0,
		5, 0,
		15, 0,
		15, 20,
		0,

		10, 10,
		15, 20,
		20, 30,
		10, 30,
		0,

		0, 20,
		10, 20,
		10, 30,
		0, 30,
		0,

		5, 10,
		5, 10,
		10, 20,
		0, 20,
		0

	};

	for (int i = 0; i < sizeof(ma) / sizeof(int) / 9; i++)
	{
		if (ma[i * 9 + 8] == 1)
			polyp(
			x0 + ma[i * 9 + 0] * (x1 - x0) / 30, y0 + ma[i * 9 + 1] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 2] * (x1 - x0) / 30, y0 + ma[i * 9 + 3] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 4] * (x1 - x0) / 30, y0 + ma[i * 9 + 5] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 6] * (x1 - x0) / 30, y0 + ma[i * 9 + 7] * (y1 - y0) / 30,
			1, 1, 1, _alpha);
		else
			polyp(
			x0 + ma[i * 9 + 0] * (x1 - x0) / 30, y0 + ma[i * 9 + 1] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 2] * (x1 - x0) / 30, y0 + ma[i * 9 + 3] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 4] * (x1 - x0) / 30, y0 + ma[i * 9 + 5] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 6] * (x1 - x0) / 30, y0 + ma[i * 9 + 7] * (y1 - y0) / 30,
			0.4, 0.4, 1.0, _alpha);
	}
}

void put_diff(int _x0, int  _y0, int  _x1, int  _y1, float _alpha)
{
	float oblxy = 1;

	int x0, y0, x1, y1;

	if ((_x1 - _x0) / oblxy> _y1 - _y0)
	{
		y0 = _y0;
		y1 = _y1;
		x0 = _x0 + (_x1 - _x0) / 2 - (y1 - y0) / 2 * oblxy;
		x1 = _x0 + (_x1 - _x0) / 2 + (y1 - y0) / 2 * oblxy;
		rect(_x0, y0, x0, y1, _alpha);
		rect(x1, y0, _x1, y1, _alpha);
	}
	else
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0 + (_y1 - _y0) / 2 - (_x1 - _x0) / 2 / oblxy;
		y1 = _y0 + (_y1 - _y0) / 2 + (_x1 - _x0) / 2 / oblxy;
		rect(x0, _y0, x1, y0, _alpha);
		rect(x0, y1, x1, _y1, _alpha);
	}
	float ots = 0.2;
	int xx0, yy0, xx1, yy1;

	xx0 = x0 + (x1 - x0) / 2 * ots;
	yy0 = y0 + (y1 - y0) / 2 * ots;
	xx1 = x1 - (x1 - x0) / 2 * ots;
	yy1 = y1 - (y1 - y0) / 2 * ots;

	rect(x0, y0, xx0, y1, _alpha);
	rect(xx1, y0, x1, y1, _alpha);
	rect(xx0, y0, xx1, yy0, _alpha);
	rect(xx0, yy0 + 3 * (yy1 - yy0) / 3, xx1, y1, _alpha);

	x0 = xx0;
	y0 = yy0;
	x1 = xx1;
	y1 = yy1;

	int ma[] =
	{
		0, 0,
		30, 0,
		30, 30,
		0, 30,
		0,

		0, 0,
		10, 0,
		10, 30,
		0, 30,
		1,

		10, 0,
		20, 0,
		30, 10,
		20, 10,
		1,

		20, 10,
		30, 10,
		20, 20,
		10, 20,
		1,

		28, 0,
		30, 0,
		30, 8,
		28, 8,
		1

	};

	for (int i = 0; i < sizeof(ma) / sizeof(int) / 9; i++)
	{
		if (ma[i * 9 + 8] == 1)
			polyp(
			x0 + ma[i * 9 + 0] * (x1 - x0) / 30, y0 + ma[i * 9 + 1] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 2] * (x1 - x0) / 30, y0 + ma[i * 9 + 3] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 4] * (x1 - x0) / 30, y0 + ma[i * 9 + 5] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 6] * (x1 - x0) / 30, y0 + ma[i * 9 + 7] * (y1 - y0) / 30,
			1, 1, 1, _alpha);
		else
			polyp(
			x0 + ma[i * 9 + 0] * (x1 - x0) / 30, y0 + ma[i * 9 + 1] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 2] * (x1 - x0) / 30, y0 + ma[i * 9 + 3] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 4] * (x1 - x0) / 30, y0 + ma[i * 9 + 5] * (y1 - y0) / 30,
			x0 + ma[i * 9 + 6] * (x1 - x0) / 30, y0 + ma[i * 9 + 7] * (y1 - y0) / 30,
			0.4, 0.4, 1.0, _alpha);
	}
}


void cl_inpu()
{
	inpu1.s = "";
	inpu2.s = "";
	inpu3.s = "";
	inpu4.s = "";
	inpu7.s = "";
	inpu5.s = "";
}

void set_N_plu()
{
	cl_inpu(); input_state = 1;
	input_type = 0; cou_title = "(Natural+0): a+b";
}

void set_N_min()
{
	cl_inpu(); input_state = 1;
	input_type = 1; cou_title = "(Natural+0): a-b";
}

void set_N_mno()
{
	cl_inpu(); input_state = 1;
	input_type = 2; cou_title = "(Natural+0): a*b";
}

void set_N_del()
{
	cl_inpu(); input_state = 1;
	input_type = 3; cou_title = "(Natural+0): a/b";
}

void set_N_mod()
{
	cl_inpu(); input_state = 1;
	input_type = 4; cou_title = "(Natural+0): a mod b";
}


void set_I_plu()
{
	cl_inpu(); input_state = 1;
	input_type = 5; cou_title = "(Integer): a+b";
}

void set_I_min()
{
	cl_inpu(); input_state = 1;
	input_type = 6; cou_title = "(Integer): a-b";
}

void set_I_mno()
{
	cl_inpu(); input_state = 1;
	input_type = 7; cou_title = "(Integer): a*b";
}

void set_I_del()
{
	cl_inpu(); input_state = 1;
	input_type = 8; cou_title = "(Integer): a/b";
}

void set_I_mod()
{
	cl_inpu(); input_state = 1;
	input_type = 9; cou_title = "(Integer): a mod b";
}


void set_Q_plu()
{
	cl_inpu(); input_state = 1;
	input_type = 10; cou_title = "(Rational): a/b+c/d";
}

void set_Q_min()
{
	cl_inpu(); input_state = 1;
	input_type = 11; cou_title = "(Rational): a/b-c/d";
}

void set_Q_mno()
{
	cl_inpu(); input_state = 1;
	input_type = 12; cou_title = "(Rational): a/b*(c/d)";
}

void set_Q_del()
{
	cl_inpu(); input_state = 1;
	input_type = 13; cou_title = "(Rational): a/b/(c/d)";
}

void set_Q_mod()
{
	cl_inpu(); input_state = 1;
	input_type = 14; cou_title = "(Rational): a mod b";
}


void set_P_plu()
{
	cl_inpu(); input_state = 1;
	input_type = 15; cou_title = "(Polynom): a+b       Ex: x2+2x+1";
}

void set_P_min()
{
	cl_inpu(); input_state = 1;
	input_type = 16; cou_title = "(Polynom): a-b       Ex: x2+2x+1";
}

void set_P_mno()
{
	cl_inpu(); input_state = 1;
	input_type = 17; cou_title = "(Polynom): a*b       Ex: x2+2x+1";
}

void set_P_del()
{
	cl_inpu(); input_state = 1;
	input_type = 18; cou_title = "(Polynom): a/b       Ex: x2+2x+1";
}

void set_P_mod()
{
	cl_inpu(); input_state = 1;
	input_type = 19; cou_title = "(Polynom): a mod b       Ex: x2+2x+1";
}


void set_O_ste()
{
	cl_inpu(); input_state = 1;
	input_type = 20; cou_title = "(Nat): a^b";
}

void set_O_ste_mod()
{
	cl_inpu(); input_state = 1;
	input_type = 21; cou_title = "(Nat): a^b mod c";
}

void set_O_diff()
{
	cl_inpu(); input_state = 1;
	input_type = 22; cou_title = "(Poly): d/dx a";
}

void set_O_nod()
{
	cl_inpu(); input_state = 1;
	input_type = 23; cou_title = "(Nat): NOD(a,b)";
}

void set_O_nok()
{
	cl_inpu(); input_state = 1;
	input_type = 24; cou_title = "(Nat): NOK(a,b)";
}

void set_O_polynod()
{
	cl_inpu(); input_state = 1;
	input_type = 25; cou_title = "(Poly): NOD(a,b)";
}

void set_O_axbyc()
{
	cl_inpu(); input_state = 1;
	input_type = 26; cou_title = "(Nat): ax+by=c";
}



/*

gui.addbutton(00, 00, 15, 20, put_ste, set_O_ste, 4);
gui.addbutton(15, 00, 45, 20, put_ste_mod, set_O_diff, 4);
gui.addbutton(45, 00, 60, 20, put_diff, set_O_ste_mod, 4);
gui.addbutton(0, 20, 15, 40, put_diff, set_O_nod, 4);
gui.addbutton(15, 20, 30, 40, put_diff, set_O_nok, 4);
gui.addbutton(30, 20, 60, 40, put_diff, set_O_polynod, 4);
gui.addbutton(00, 40, 30, 60, put_diff, set_O_axbyc, 4);

*/
/*

void TimerInit(void)
{
memset(&mytimer, 0, sizeof(mytimer));

if (!QueryPerformanceFrequency((LARGE_INTEGER *) &mytimer.frequency))
{
// Нет высокоточного таймера
mytimer.performance_timer  = FALSE;
mytimer.mm_timer_start  = timeGetTime();
mytimer.resolution  = 1.0f/1000.0f;
mytimer.frequency    = 1000;
mytimer.mm_timer_elapsed  = mytimer.mm_timer_start;
}
else
{
// Высокоточный таймер доступен, используем его вместо мультимедийного таймера
// Взять текущее время и сохранить его в performance_timer_start
QueryPerformanceCounter((LARGE_INTEGER *) &mytimer.performance_timer_start);
mytimer.performance_timer    = TRUE;        // Установить флаг наличия таймера в TRUE
// Вычислить точность таймера, используя частоту
mytimer.resolution    = (float) (((double)1.0f)/((double)mytimer.frequency));
// Присвоить прошедшему времени текущее время
mytimer.performance_timer_elapsed  = mytimer.performance_timer_start;
}
}

float TimerGetTim()           // Взять время в миллисекундах
{
__int64 ztime;                // time содержит 64 бита
__int64 time;                // time содержит 64 бита

if (mytimer.performance_timer) // Есть высокоточный таймер?
{
// Захват текущего значения высокоточного таймера
QueryPerformanceCounter((LARGE_INTEGER *) &time);
// Вернем текущее время минус начальное время, умноженное на точность и 1000 (для миллисекунд)
ztime=mytimer.performance_timer_start;
mytimer.performance_timer_start=time;
return ( (float) ( time - ztime) * mytimer.resolution)*1000.0f;
}
else
{
// Вернем текущее время минус начальное время, умноженное на точность и 1000 (для миллисекунд)
ztime=mytimer.mm_timer_start;
mytimer.mm_timer_start=timeGetTime();
return( (float) ( timeGetTime() - mytimer.mm_timer_start) * mytimer.resolution)*1000.0f;
}
}

*/

GLvoid BuildFont(float heigf)								// Build Our Bitmap Font
{
	heigf = 30;
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(heigf,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		L"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(HDc, font);           // Selects The Font We Want
	wglUseFontBitmaps(HDc, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(HDc, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void TerminateApplication(GL_Window* window)							// Terminate The Application
{
	PostMessage(window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory(&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize = sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth = width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight = height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel = bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	return TRUE;														// Display Change Was Successful, Return True
}

void ReshapeGL(int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity();													// Reset The Projection Matrix
	gluPerspective(90.0f, (GLfloat)(width) / (GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity();													// Reset The Modelview Matrix
	KillFont();
	BuildFont(-height / 40.0f);
	razokx = width;
	razoky = height;

	gui.resize(razokx, razoky);

	inpu1 = INP(10, razoky / 2 - 50, razokx - 10, razoky / 2 - 20, "a:");
	inpu2 = INP(10, razoky / 2 - 10, razokx - 10, razoky / 2 + 20, "b:");
	inpu3 = INP(10, razoky / 2 + 30, razokx - 10, razoky / 2 + 60, "c:");
	inpu4 = INP(10, razoky / 2 + 70, razokx - 10, razoky / 2 + 100, "d:");
	inpu7 = INP(razokx / 2 - 80, razoky / 2 + 110, razokx / 2 + 80, razoky / 2 + 140, "CALCULATE");
	inpu5 = INP(10, razoky / 2 + 150, razokx - 10, razoky / 2 + 180, "RES:");
	inpu6 = INP(razokx / 2 - 50, razoky / 2 + 190, razokx / 2 + 50, razoky / 2 + 220, "Back");

	font_x = 30;
}

int dvavx(int i)
{
	int r, o = 2;
	for (r = 1; r<i; r++)
		o *= 2;
	return o;
}

float min1(float i, float r)
{
	if (i>r)return r;
	return i;
}

float abs1(float i)
{
	if (i<0)return -i;
	return i;
}

float max1(float i, float r)
{
	if (i>r)return i;
	return r;
}


BOOL CreateWindowGL(GL_Window* window)									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		window->init.bitsPerPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect = { 0, 0, window->init.width, window->init.height };	// Define Our Window Coordinates

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format

	if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		if (ChangeScreenResolution(window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// Fullscreen Mode Failed.  Run In Windowed Mode Instead
			MessageBox(HWND_DESKTOP, L"Mode Switch Failed.\nRunning In Windowed Mode.", L"Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
		}
		else															// Otherwise (If Fullscreen Mode Was Successful)
		{
			//ShowCursor (FALSE);											// Turn Off The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
	}
	else																// If Fullscreen Was Not Selected
	{
		// Adjust Window, Account For Window Borders
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// Create The OpenGL Window
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// Extended Style
		(LPCWSTR)window->init.application->className,	// Class Name
		(LPCWSTR)window->init.title,					// Window Title
		windowStyle,							// Window Style
		0, 0,								// Window X,Y Position
		windowRect.right - windowRect.left,	// Window Width
		windowRect.bottom - windowRect.top,	// Window Height
		HWND_DESKTOP,						// Desktop Is Window's Parent
		0,									// No Menu
		window->init.application->hInstance, // Pass The Window Instance
		window);

	if (window->hWnd == 0)												// Was Window Creation A Success?
	{
		return FALSE;													// If Not Return False
	}

	window->hDC = GetDC(window->hWnd);									// Grab A Device Context For This Window
	if (window->hDC == 0)												// Did We Get A Device Context?
	{
		// Failed
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	PixelFormat = ChoosePixelFormat(window->hDC, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if (SetPixelFormat(window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	window->hRC = wglCreateContext(window->hDC);						// Try To Get A Rendering Context
	if (window->hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent(window->hDC, window->hRC) == FALSE)
	{
		// Failed
		wglDeleteContext(window->hRC);									// Delete The Rendering Context
		window->hRC = 0;												// Zero The Rendering Context
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	ShowWindow(window->hWnd, SW_NORMAL);								// Make The Window Visible
	window->isVisible = TRUE;											// Set isVisible To True

	ReshapeGL(window->init.width, window->init.height);				// Reshape Our GL Window

	ZeroMemory(window->keys, sizeof (Keys));							// Clear All Keys

	window->lastTickCount = GetTickCount();							// Get Tick Count




	HDc = window->hDC;


	return TRUE;														// Window Creating Was A Success
	// Initialization Will Be Done In WM_CREATE
}

BOOL DestroyWindowGL(GL_Window* window)								// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent(window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext(window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context
			}
			ReleaseDC(window->hWnd, window->hDC);						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings(NULL, 0);									// Switch Back To Desktop Resolution
		ShowCursor(TRUE);												// Show The Cursor
	}
	return TRUE;														// Return True
}

bool LoadTGA(TextureImage *texture, char *filename, int fff)				// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type = GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if (file == NULL ||											// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||									// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||									// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp = header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize = (texture->width)*(texture->height)*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	//imageSize	=128*128*128;	
	texture->imageData = (GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, fff);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, fff);	// Linear Filtered

	if (texture[0].bpp == 24)										// Was The TGA 24 Bits
	{
		type = GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD				tickCount;										// Used For The Tick Counter

	// Get The Window Context
	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));

	switch (uMsg)														// Evaluate Window Message
	{
	case WM_PAINT:													// Window Needs Updating
	{
																		tickCount = GetTickCount();								// Get The Tick Count
																		Update(tickCount - window->lastTickCount);					// Update The Counter
																		window->lastTickCount = tickCount;							// Set Last Count To Current Count
																		Draw();													// Draw Our Scene
																		SwapBuffers(window->hDC);									// Swap Buffers (Double Buffering)
	}
		return 0;														// Return
	case WM_MOUSEMOVE:
		mxz = mx;
		myz = my;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		mchl = 1;
		break;
	case WM_LBUTTONUP:
		mchl = 0;
		break;
	case WM_RBUTTONDOWN:
		mchr = 1;
		break;
	case WM_RBUTTONUP:
		mchr = 0;
		break;

	case WM_SYSCOMMAND:												// Intercept System Commands
	{
																		switch (wParam)												// Check System Calls
																		{
																		case SC_SCREENSAVE:										// Screensaver Trying To Start?
																		case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
																			return 0;												// Prevent From Happening
																		}
																		break;														// Exit
	}
		return 0;														// Return

	case WM_CREATE:													// Window Creation
	{
																		CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
																		window = (GL_Window*)(creation->lpCreateParams);
																		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(window));



	}
		return 0;														// Return
	case WM_CLOSE:													// Closing The Window
		TerminateApplication(window);								// Terminate The Application
		return 0;														// Return

	case WM_SIZE:													// Size Action Has Taken Place
		switch (wParam)												// Evaluate Size Action
		{
		case SIZE_MINIMIZED:									// Was Window Minimized?
			window->isVisible = FALSE;							// Set isVisible To False
			return 0;												// Return

		case SIZE_MAXIMIZED:									// Was Window Maximized?
			window->isVisible = TRUE;							// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));		// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return

		case SIZE_RESTORED:										// Was Window Restored?
			window->isVisible = TRUE;							// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));		// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return
		}
		break;															// Break


	case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			/*
			if (wParam == 'Q')
			{
				if (submenu == 0)
				{
					submenu = 1;
				}
				else{
					submenu = 0;
				}

			}
			*/
			window->keys->keyDown[wParam] = TRUE;					// Set The Selected Key (wParam) To True
			return 0;												// Return
		}
		break;															// Break

	case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			window->keys->keyDown[wParam] = FALSE;					// Set The Selected Key (wParam) To False
			return 0;												// Return
		}
		break;															// Break

	case WM_TOGGLEFULLSCREEN:										// Toggle FullScreen Mode On/Off
		g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;															// Break
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

BOOL RegisterWindowClass(Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory(&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize = sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc = (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance = application->hInstance;				// Set The Instance
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW + 1;				// Class Background Brush Color
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName = (LPCWSTR)application->className;				// Sets The Applications Classname
	if (RegisterClassEx(&windowClass) == 0)							// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox(HWND_DESKTOP, L"RegisterClassEx Failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int i, r, o, j;

	gui.addbutton(00, 00, 20, 30, put_N, NULL, -1);
	gui.addbutton(20, 00, 40, 30, put_I, NULL, -1);
	gui.addbutton(40, 00, 60, 30, put_R, NULL, -1);
	gui.addbutton(00, 30, 20, 60, put_P, NULL, -1);
	gui.addbutton(20, 30, 40, 60, put_O, NULL, -1);
	gui.addbutton(40, 30, 60, 60, put_EX, (void(*)())exit, -1);

	gui.addbutton(00, 00, 20, 30, put_plu, set_N_plu, 0);
	gui.addbutton(20, 00, 40, 30, put_min, set_N_min, 0);
	gui.addbutton(40, 00, 60, 30, put_mno, set_N_mno, 0);
	gui.addbutton(00, 30, 20, 60, put_del, set_N_del, 0);
	gui.addbutton(20, 30, 40, 60, put_mod, set_N_mod, 0);
	gui.addbutton(40, 30, 60, 60, put_back, (void(*)())exit, 0);

	gui.addbutton(00, 00, 20, 30, put_plu, set_I_plu, 1);
	gui.addbutton(20, 00, 40, 30, put_min, set_I_min, 1);
	gui.addbutton(40, 00, 60, 30, put_mno, set_I_mno, 1);
	gui.addbutton(00, 30, 20, 60, put_del, set_I_del, 1);
	gui.addbutton(20, 30, 40, 60, put_mod, set_I_mod, 1);
	gui.addbutton(40, 30, 60, 60, put_back, (void(*)())exit, 1);

	gui.addbutton(00, 00, 20, 30, put_plu, set_Q_plu, 2);
	gui.addbutton(20, 00, 40, 30, put_min, set_Q_min, 2);
	gui.addbutton(40, 00, 60, 30, put_mno, set_Q_mno, 2);
	gui.addbutton(00, 30, 20, 60, put_del, set_Q_del, 2);

	gui.addbutton(40, 30, 60, 60, put_back, (void(*)())exit, 2);

	gui.addbutton(00, 00, 20, 30, put_plu, set_P_plu, 3);
	gui.addbutton(20, 00, 40, 30, put_min, set_P_min, 3);
	gui.addbutton(40, 00, 60, 30, put_mno, set_P_mno, 3);
	gui.addbutton(00, 30, 20, 60, put_del, set_P_del, 3);
	gui.addbutton(20, 30, 40, 60, put_mod, set_P_mod, 3);
	gui.addbutton(40, 30, 60, 60, put_back, (void(*)())exit, 3);

	gui.addbutton(00, 00, 15, 20, put_ste, set_O_ste, 4);
	gui.addbutton(15, 00, 45, 20, put_ste_mod, set_O_ste_mod, 4);
	gui.addbutton(45, 00, 60, 20, put_diff, set_O_diff, 4);
	gui.addbutton(0, 20, 15, 40, put_diff, set_O_nod, 4);
	gui.addbutton(15, 20, 30, 40, put_diff, set_O_nok, 4);
	gui.addbutton(30, 20, 60, 40, put_diff, set_O_polynod, 4);
	gui.addbutton(00, 40, 30, 60, put_diff, set_O_axbyc, 4);
	gui.addbutton(30, 40, 60, 60, put_back, (void(*)())exit, 4);


	Application			application;									// Window Structure
	Keys				keys;											// Key Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure

	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window
	ZeroMemory(&window, sizeof (GL_Window));							// Make Sure Memory Is Zeroed
	window.keys = &keys;								// Window Key Structure
	window.init.application = &application;							// Window Application

	// Window Title
	window.init.title = "D\0i\0s\0c\0r\0e\0t\0e\0 \0m\0a\0t\0h\0 \0s\0a\0m\0r\0r\0r\0@\0m\0a\0i\0l\0.\0r\0u\0";		// Tutorial Title


	window.init.width = razokx;									// Window Width
	window.init.height = razoky;									// Window Height
	window.init.bitsPerPixel = 32;									// Bits Per Pixel
	window.init.isFullScreen = TRUE;									// Fullscreen? (Set To TRUE)

	ZeroMemory(&keys, sizeof (Keys));									// Zero keys Structure

	// Ask The User If They Want To Start In FullScreen Mode? (Remove These 4 Lines If You Want To Force Fullscreen)
	if (MessageBox(HWND_DESKTOP, L"Would You Like To Run In Fullscreen Mode?", L"Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		razokx = 640;
		razoky = 480;
		window.init.width = 640;
		window.init.height = 480;
		window.init.isFullScreen = FALSE;								// If Not, Run In Windowed Mode
	}

	// Register A Class For Our Window To Use
	if (RegisterWindowClass(&application) == FALSE)					// Did Registering A Class Fail?
	{
		// Failure
		MessageBox(HWND_DESKTOP, L"Error Registering Window Class!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen Is Set To User Default
	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Window
		window.init.isFullScreen = g_createFullScreen;					// Set Init Param Of Window Creation To Fullscreen?
		if (CreateWindowGL(&window) == TRUE)							// Was Window Creation Successful?
		{
			// At This Point We Should Have A Window That Is Setup To Render OpenGL
			if (Initialize(&window, &keys) == FALSE)					// Call User Initialization
			{
				// Failure
				TerminateApplication(&window);							// Close Window, This Will Handle The Shutdown
			}
			else														// Otherwise (Start The Message Pump)
			{	// Initialize was a success
				isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
				while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
				{
					// Success Creating Window.  Check For Window Messages
					if (PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Check For WM_QUIT Message
						if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
						{
							DispatchMessage(&msg);						// If Not, Dispatch The Message
						}
						else											// Otherwise (If Message Is WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Terminate The Message Pump
						}
					}
					else												// If There Are No Messages
					{
						if (window.isVisible == FALSE)					// If Window Is Not Visible
						{
							WaitMessage();								// Application Is Minimized Wait For A Message
						}
					}
				}														// Loop While isMessagePumpActive == TRUE
			}															// If (Initialize (...

			// Application Is Finished
			Deinitialize();											// User Defined DeInitialization

			DestroyWindowGL(&window);									// Destroy The Active Window
		}
		else															// If Window Creation Failed
		{
			// Error Creating Window
			MessageBox(HWND_DESKTOP, L"Error Creating OpenGL Window", L"Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass((LPCWSTR)application.className, application.hInstance);		// UnRegister Window Class
	return 0;
}																		// End Of WinMain()

void ToggleFullscreen(GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
}

void putknopk(float x, float y, float x1, float y1, char* textinkn)
{
	int i, r, o, j, q, w, a, b, c, d;


	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(x - 1, 1 - (y), -1);
	glVertex3f(x - 1, 1 - (y1), -1);
	glVertex3f(x1 - 1, 1 - (y1), -1);
	glVertex3f(x1 - 1, 1 - (y), -1);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(x + 0.01 - 1, 1 - (y + 0.01), -1);
	glVertex3f(x + 0.01 - 1, 1 - (y1 - 0.01), -1);
	glVertex3f(x1 - 0.01 - 1, 1 - (y1 - 0.01), -1);
	glVertex3f(x1 - 0.01 - 1, 1 - (y + 0.01), -1);
	glEnd();

	glColor3f(0, 0, 0);
	glRasterPos3f(x + 0.015 - 1, 1 - (y1 + y) / 2 - 0.014, -1);
	glPrint(textinkn);

}

int mxv(float x, float y, float x1, float y1)

{
	if ((float)(razoky / 2.0 - my) / (float)razoky * 2>min1(y, y1))
	if ((float)(razoky / 2.0 - my) / (float)razoky * 2<max1(y, y1))
	if ((float)(mx - razokx / 2.0) / (float)razoky * 2>min1(x, x1))
	if ((float)(mx - razokx / 2.0) / (float)razoky * 2<max1(x, x1))
		return 1;
	return 0;
}

int BuildTexture(char *szPathName, GLuint &texid)						// Load Image And Convert To A Texture
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH + 1];									// Full Path To Picture (WCHAR)
	char		szPath[MAX_PATH + 1];	// Full Path To Picture
	TCHAR		szPathDD[MAX_PATH + 1];
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim;											// Holds Maximum Texture Size

	if (strstr(szPathName, "http://"))									// If PathName Contains http:// Then...
	{
		strcpy(szPath, szPathName);										// Append The PathName To szPath
	}
	else																// Otherwise... We Are Loading From A File
	{
		//GetCurrentDirectory(MAX_PATH, szPathDD);							// Get Our Working Directory
		CharToOem(szPathDD, szPath);
		strcat(szPath, "\\");											// Append "\" After The Working Directory
		strcat(szPath, szPathName);										// Append The PathName
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if (FAILED(hr))														// If Loading Failed
		return FALSE;													// Return False

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if (!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	//	Create A Temporary Bitmap
	BITMAPINFO	bi = { 0 };												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount = 32;									// 32 Bit
	bi.bmiHeader.biWidth = lWidthPixels;							// Power Of Two Width
	bi.bmiHeader.biHeight = lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression = BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes = 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

	if (!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	for (long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
	{
		BYTE* pPixel = (BYTE*)(&pBits[i]);							// Grab The Current Pixel
		BYTE  temp = pPixel[0];									// Store 1st Color In Temp Variable (Blue)
		pPixel[0] = pPixel[2];									// Move Red Value To Correct Position (1st)
		pPixel[2] = temp;											// Move Temp Value To Correct Blue Position (3rd)
		pPixel[3] = 255;											// Set The Alpha Value To 255
	}

	glGenTextures(1, &texid);											// Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// (Modify This For The Type Of Filtering You Want)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);     // (Modify This For The Type Of Filtering You Want)

	// (Modify This If You Want Mipmaps)
	glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

	DeleteObject(hbmpTemp);												// Delete The Object
	DeleteDC(hdcTemp);													// Delete The Device Context

	pPicture->Release();												// Decrements IPicture Reference Count
	//}else{return FALSE;};
	return TRUE;														// Return True (All Good)
}

BOOL Initialize(GL_Window* window, Keys* keys)							// Any GL Init Code & User Initialiazation Goes Here
{
	char s[100];
	char destination[65];

	int i, r, o, j, q, w;
	g_window = window;												// Window Values
	g_keys = keys;													// Key Values


	//TimerInit();

	LoadTGA(&textureTGA[0], "Data/bmp/0.tga", GL_LINEAR);
	LoadTGA(&textureTGA[6], "Data/bmp/6.tga", GL_LINEAR);

	/*
	for (i = 1; i <= 5; i++)
	{
		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "icon.tga");
		LoadTGA(&blockinfo[i].textureTGA, destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu0.tga");
		LoadTGA(&blocktex[i][0], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu1.tga");
		LoadTGA(&blocktex[i][1], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu2.tga");
		LoadTGA(&blocktex[i][2], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu3.tga");
		LoadTGA(&blocktex[i][3], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu4.tga");
		LoadTGA(&blocktex[i][4], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu5.tga");
		LoadTGA(&blocktex[i][5], destination, GL_LINEAR);

	}
	*/
	/*
	LoadTGA(&blockinfo[1].textureTGA, "Data/bmp/block1icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][0], "Data/bmp/block1tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][1], "Data/bmp/block1tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][2], "Data/bmp/block1tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][3], "Data/bmp/block1tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][4], "Data/bmp/block1tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][5], "Data/bmp/block1tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[2].textureTGA, "Data/bmp/block2icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][0], "Data/bmp/block2tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][1], "Data/bmp/block2tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][2], "Data/bmp/block2tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][3], "Data/bmp/block2tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][4], "Data/bmp/block2tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][5], "Data/bmp/block2tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[3].textureTGA, "Data/bmp/block3icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][0], "Data/bmp/block3tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][1], "Data/bmp/block3tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][2], "Data/bmp/block3tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][3], "Data/bmp/block3tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][4], "Data/bmp/block3tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][5], "Data/bmp/block3tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[4].textureTGA, "Data/bmp/block4icon.tga", GL_LINEAR);
	*/
	LoadTGA(&basetex[0], "Data/bmp/base0.tga", GL_LINEAR);

	glEnable(GL_TEXTURE_2D);											// Enable Texture Mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);								// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing
	glEnable(GL_DEPTH_TEST);											// Enable Depth Testing
	glShadeModel(GL_SMOOTH);											// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);					// Set Perspective Calculations To Most Accurate


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.99f);
	glDisable(GL_ALPHA_TEST);






	BuildFont(-razoky / 40.0f);


	return TRUE;
}

void Deinitialize(void)
{
}

void Update(DWORD milliseconds)
{
	int i, r, o, j, q, w, a, b, c, d;
	if (exitgame == 1)
	{
		TerminateApplication(g_window);
	}

	if (input_state == 0)
		gui.upd(mx, my, mchl == 1 && mchlz == 0, milliseconds);
	if (input_state == 1)
	{
		count_res(mx, my, mchl == 1 && mchlz == 0);
	}
		/*
	if (g_keys->keyDown[VK_ESCAPE])
	{
		if (gamemen == 1)
		{
			baseship = ship[myshipid];
		}
		gamemen = 3;
		//exitgame=1;
	}
	*/

	if (g_keys->keyDown[VK_F1])
	{
		if (!g_createFullScreen)
		{
			window.init.width = GetSystemMetrics(SM_CXSCREEN);
			window.init.height = GetSystemMetrics(SM_CYSCREEN);
		}
		else{
			window.init.width = 150;
			window.init.height = 150;
		}

		ToggleFullscreen(g_window);
	}

	if (g_keys->keyDown['S'])
		movveforw(cam.c2, cam.c1, cam.c3, -0.2, &cam.x, &cam.y, &cam.z);
	if (g_keys->keyDown['W'])
		movveforw(cam.c2, cam.c1, cam.c3, 0.2, &cam.x, &cam.y, &cam.z);

}

void circl(float x, float y, float z, float c)
{
	int i, r, o, j, q, w, a, b, d;
	float xx, yy, x1, y1, x2, y2, x3, y3, u, v;
	glDisable(GL_TEXTURE_2D);


	glBegin(GL_POLYGON);

	o = 10;
	for (i = 0; i<o; i++)
	{
		xx = ddx(i * 360 / o, c);
		yy = ddy(i * 360 / o, c);
		glVertex3f(x + xx, y, z + yy);
		//		glVertex3f(x+rand()%10,y+rand()%10,z+rand()%10);
	}

	glEnd();


}

float movec(float c, float cnu, float gr)
{

	int i, r, o, j, q, w;
	float u, v, xx, yy;
	xx = c;

	q = 0;
	if (c >= 360) c -= 360;
	if (c  <   0) c += 360;
	if (cnu >= 360) cnu -= 360;
	if (cnu<   0) cnu += 360;

	if (min1(min1(min1(abs1(cnu - c - 360), abs1(c - cnu - 360)),
		min1(abs1(cnu - c), abs1(c - cnu))),
		min1(abs1(cnu - c + 360), abs1(c - cnu + 360))


		)<abs1(gr))
	{
		q = 1; xx = cnu;
	}

	if (q == 0)
	{
		if (abs(cnu - c)<180)
		{
			if (c - cnu>0){ xx = c - gr; }
			else{ xx = c + gr; }
		}
		else{
			if (c  >180){ xx = c + gr; }
			else{ xx = c - gr; }
		}
	}








	if (xx >= 360) xx -= 360;
	if (xx  <   0) xx += 360;
	return xx;

}

void getvec3d(float cam_c, float cam_c1, float cam_c2, float *x, float *y, float *z)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.y, &raycam.z, 0, 0, cam_c2);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c);

	z[0] = -raycam.x;
	x[0] = raycam.y;
	y[0] = -raycam.z;

}

void Draw()
{

	int desem, i, r, o, j, q, w, a, b, c, d, tt1, tt2, tt3, tt4;
	float xx, yy, zz, x1, y1, z1, x2, y2, z2, x3, y3, z3, u, v;


	glDisable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 1);

	ti++;
	/*/
	mxz = razokx / 2;
	myz = razoky / 2;
	if (((mx - mxz) != 0) || ((my - myz) != 0))
	{
		movve(&cam.c2, &cam.c1, &cam.c3, -(mx - mxz), (my - myz));
		SetCursorPos(razokx / 2, razoky / 2);
	}
	glPushMatrix();

	glRotatef(cam.c3, 0, 0, 1);
	glRotatef(cam.c1, 1, 0, 0);
	glRotatef(cam.c2, 0, 1, 0);
	glTranslatef(-cam.x, -cam.y, -cam.z);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textureTGA[0].texID);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(10, 20, 40);
	glTexCoord2f(0, 1); glVertex3f(10, 20, 10);
	glTexCoord2f(1, 1); glVertex3f(40, 20, 10);
	glTexCoord2f(1, 0); glVertex3f(40, 20, 40);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(10, -20, 40);
	glTexCoord2f(0, 1); glVertex3f(10, -20, 10);
	glTexCoord2f(1, 1); glVertex3f(40, -20, 10);
	glTexCoord2f(1, 0); glVertex3f(40, -20, 40);
	glEnd();

	glPopMatrix();

	/**/
	//gui.upd();


	if (input_state == 0)
		gui.draw();
	if (input_state == 1)
		count_res_draw();

	//rect(0, 0, razokx, razoky,1);

	/*

	glDisable(GL_TEXTURE_2D);
	//END Graphics


	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	glColor4f(1, 0.7, 0.7, 1);
	glBegin(GL_LINES);
	glVertex3f(-0.1, 0, -1);
	glVertex3f(0.1, 0, -1);
	glVertex3f(0, -0.1, -1);
	glVertex3f(0, 0.1, -1);
	glEnd();

	glColor4f(1, 1, 1, 1);
	glRasterPos3f(0, 0, -1);
	//	glPrint("Milliseconds:%f",0.0001);


	*/

	mchz = mch;
	mchlz = mchl;
	mchrz = mchr;


}



void movve(float *cam_c, float *cam_c1, float *cam_c2, float x, float y)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.x, &raycam.z, 0, 0, y);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, y);
	roate(&raycamup.x, &raycamup.z, 0, 0, y);

	roate(&raycam.x, &raycam.y, 0, 0, -x);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, -x);
	roate(&raycamup.x, &raycamup.y, 0, 0, -x);
	//sdelal samrrr@mail.ru
	roate(&raycam.y, &raycam.z, 0, 0, cam_c2[0]);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2[0]);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2[0]);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1[0]);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1[0]);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1[0]);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c[0]);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c[0]);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c[0]);

	//	camv.x=raycam.y;
	//	camv.y=-raycam.z;
	//	camv.z=-raycam.x;

	/*
	//xyz
	//xzy
	//yxz
	//yzx
	//zxy
	//zyx
	*/

	u = ss(0, 0, raycam.y, raycam.x);
	u = -u;
	cam_c[0] = u;

	roate(&raycam.x, &raycam.y, 0, 0, -u);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, -u);
	roate(&raycamup.x, &raycamup.y, 0, 0, -u);

	u = ss(0, 0, raycam.x, raycam.z);
	u = u + 90 + 180;
	cam_c1[0] = u;

	roate(&raycam.x, &raycam.z, 0, 0, -u);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, -u);
	roate(&raycamup.x, &raycamup.z, 0, 0, -u);

	u = ss(0, 0, raycamup.y, raycamup.z);

	cam_c2[0] = u;

	//roate(&raycam    .y,&raycam    .z,0,0,-u);
	//roate(&raycamleft.y,&raycamleft.z,0,0,-u);
	//roate(&raycamup  .y,&raycamup  .z,0,0,-u);


}

void movveforw(float cam_c, float cam_c1, float cam_c2, float rass, float *cam_x, float *cam_y, float *cam_z)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.y, &raycam.z, 0, 0, cam_c2);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c);

	cam_z[0] -= raycam.x*rass;
	cam_x[0] += raycam.y*rass;
	cam_y[0] -= raycam.z*rass;



}

void calcu()
{
	if (input_type == 0)
	{
		inpu5.s = ((BigNat(inpu1.s) + inpu2.s)).ToString();
	}
	if (input_type == 1)
	{
		inpu5.s = ((BigNat(inpu1.s) - inpu2.s)).ToString();
	}
	if (input_type == 2)
	{
		inpu5.s = ((BigNat(inpu1.s) * inpu2.s)).ToString();
	}
	if (input_type == 3)
	{
		inpu5.s = ((BigNat(inpu1.s) / inpu2.s)).ToString();
	}
	if (input_type == 4)
	{
		inpu5.s = ((BigNat(inpu1.s) % inpu2.s)).ToString();
	}


	if (input_type == 5)
	{
		inpu5.s = ((BigInt(inpu1.s) + inpu2.s)).ToString();
	}
	if (input_type == 6)
	{
		inpu5.s = ((BigInt(inpu1.s) - inpu2.s)).ToString();
	}
	if (input_type == 7)
	{
		inpu5.s = ((BigInt(inpu1.s) * inpu2.s)).ToString();
	}
	if (input_type == 8)
	{
		inpu5.s = ((BigInt(inpu1.s) / inpu2.s)).ToString();
	}
	if (input_type == 9)
	{
		inpu5.s = ((BigInt(inpu1.s) % inpu2.s)).ToString();
	}


	if (input_type == 10)
	{
		inpu5.s = ((BigRat(inpu1.s, inpu2.s) + BigRat(inpu3.s, inpu4.s))).ToString();
	}
	if (input_type == 11)
	{
		inpu5.s = ((BigRat(inpu1.s, inpu2.s) - BigRat(inpu3.s, inpu4.s))).ToString();
	}
	if (input_type == 12)
	{
		inpu5.s = ((BigRat(inpu1.s, inpu2.s) * BigRat(inpu3.s, inpu4.s))).ToString();
	}
	if (input_type == 13)
	{
		inpu5.s = ((BigRat(inpu1.s, inpu2.s) / BigRat(inpu3.s, inpu4.s))).ToString();
	}
	if (input_type == 14)
	{
		//inpu5.s = ((BigInt(inpu1.s) % inpu2.s)).ToString();
	}


	if (input_type == 15)
	{
		inpu5.s = ((BigPol(inpu1.s) + inpu2.s)).ToString();
	}
	if (input_type == 16)
	{
		inpu5.s = ((BigPol(inpu1.s) - inpu2.s)).ToString();
	}
	if (input_type == 17)
	{
		inpu5.s = ((BigPol(inpu1.s) * inpu2.s)).ToString();
	}
	if (input_type == 18)
	{
		inpu5.s = ((BigPol(inpu1.s) / inpu2.s)).ToString();
	}
	if (input_type == 20)
	{
		int i=atoi(inpu2.s.data());
		inpu5.s = (pow((BigNat)inpu1.s,i)).ToString();
	}
	if (input_type == 21)
	{
		inpu5.s = (powmod((BigNat)inpu1.s, (BigNat)inpu2.s, (BigNat)inpu3.s)).ToString();
	}
	if (input_type == 22)
	{
		inpu5.s = ( ((BigPol)("0")).deg((BigPol)inpu1.s) ).ToString();
	}
	if (input_type == 23)
	{
		inpu5.s = (NOD((BigNat)inpu1.s, (BigNat)inpu2.s)).ToString();
	}
	if (input_type == 24)
	{
		inpu5.s = (NOK((BigNat)inpu1.s, (BigNat)inpu2.s)).ToString();
	}
	if (input_type == 25)
	{
		inpu5.s = (POLY_NOD((BigPol)inpu1.s, (BigPol)inpu2.s)).ToString();
	}
	if (input_type == 26)
	{
		//inpu5.s = ().ToString();
	}



}

void count_res(int _mx,int _my,int _mch)
{
	char ch;



	ch = 0;

	if (g_keys->keyDown['\b'])ch = '\b';
	if (g_keys->keyDown['X'])ch = 'x';
	if (g_keys->keyDown[VK_DIVIDE])ch = '/';
	if (g_keys->keyDown[VK_SUBTRACT] || !g_keys->keyDown[VK_SHIFT] && g_keys->keyDown[0xBD])ch = '-';
	if (g_keys->keyDown[VK_ADD] || g_keys->keyDown[VK_SHIFT] && g_keys->keyDown[0xBB])ch = '+';

	if (g_keys->keyDown['0'])ch = '0';
	if (g_keys->keyDown['1'])ch = '1';
	if (g_keys->keyDown['2'])ch = '2';
	if (g_keys->keyDown['3'])ch = '3';
	if (g_keys->keyDown['4'])ch = '4';
	if (g_keys->keyDown['5'])ch = '5';
	if (g_keys->keyDown['6'])ch = '6';
	if (g_keys->keyDown['7'])ch = '7';
	if (g_keys->keyDown['8'])ch = '8';
	if (g_keys->keyDown['9'])ch = '9';

	g_keys->keyDown['\b'] = 0;
	g_keys->keyDown['X'] = 0;
	g_keys->keyDown[VK_SUBTRACT] = 0;
	g_keys->keyDown[VK_ADD] = 0;
	if (g_keys->keyDown[VK_SHIFT] && g_keys->keyDown[0xBB])
		g_keys->keyDown[0xBB] = 0;
	if (!g_keys->keyDown[VK_SHIFT] && g_keys->keyDown[0xBD])
		g_keys->keyDown[0xBD] = 0;
	g_keys->keyDown[VK_DIVIDE] = 0;

	g_keys->keyDown['0'] = 0;
	g_keys->keyDown['1'] = 0;
	g_keys->keyDown['2'] = 0;
	g_keys->keyDown['3'] = 0;
	g_keys->keyDown['4'] = 0;
	g_keys->keyDown['5'] = 0;
	g_keys->keyDown['6'] = 0;
	g_keys->keyDown['7'] = 0;
	g_keys->keyDown['8'] = 0;
	g_keys->keyDown['9'] = 0;

	if (ch != 0)
	{
		inpu1.addsymbol(ch);
		inpu2.addsymbol(ch);
		inpu3.addsymbol(ch);
		inpu4.addsymbol(ch);
	}

	if (_mch == 1)
	{
		inpu1.selected = 0;
		inpu2.selected = 0;
		inpu3.selected = 0;
		inpu4.selected = 0;
		inpu7.selected = 0;

		if (mx > inpu1.x0 && my > inpu1.y0 && mx < inpu1.x1 && my < inpu1.y1)
			inpu1.selected = 1;
		if (mx > inpu2.x0 && my > inpu2.y0 && mx < inpu2.x1 && my < inpu2.y1)
			inpu2.selected = 1;
		if (mx > inpu3.x0 && my > inpu3.y0 && mx < inpu3.x1 && my < inpu3.y1)
			inpu3.selected = 1;
		if (mx > inpu4.x0 && my > inpu4.y0 && mx < inpu4.x1 && my < inpu4.y1)
			inpu4.selected = 1;
		if (mx > inpu7.x0 && my > inpu7.y0 && mx < inpu7.x1 && my < inpu7.y1)
		{
			inpu7.selected = 1;
			inpu7.s_b = "Wait...";
		}
		if (mx > inpu6.x0 && my > inpu6.y0 && mx < inpu6.x1 && my < inpu6.y1)
			input_state = 0;
	}
	if (inpu7.selected == 2)
	{
		calcu();
		inpu7.selected = 0;
		inpu7.s_b = "CALCULATE";
	}
	if (inpu7.selected == 1)
		inpu7.selected = 2;

}

void count_res_draw()
{
	inpu1.draw();
	if (input_type!=22)
	inpu2.draw();
	switch (input_type)
	{
	case 10:
	case 11:
	case 12:
	case 13:
		inpu3.draw();
		inpu4.draw();
		break;
	case 26:
	case 21:
		inpu3.draw();
		break;
	}
	inpu7.draw();
	inpu5.draw();
	inpu6.draw();
	glColor4f(1,1,1, 1);
	glRasterPos3f(0 - cou_title.size() / 2.0 / razoky*font_x, 0.50, -1);
	glPrint(cou_title.data());

}
