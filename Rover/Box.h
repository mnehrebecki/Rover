#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>

class Box
{
public:
	Box(double length, double height, double width, GLfloat wsp[]);
	~Box();
	double szer;
	double wys;
	double gleb;
	GLfloat poczatek[3];
	void draw();
};

