#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>
class Lamp
{
public:
	Lamp(double r, double h, GLfloat wsp[3]);
	~Lamp();

	double r;
	double h;
	GLfloat srodek[3];
	void draw();
};

