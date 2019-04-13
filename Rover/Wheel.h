#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>

class Wheel
{
public:
	double r;
	double h; 
	GLfloat srodek[3];
	Wheel(double r,double h, GLfloat wsp[3]);
	~Wheel();
	void draw();
};

