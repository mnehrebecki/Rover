#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>
class Roller
{
public:
	double r;
	double h;
	GLfloat srodek[3];
	Roller(double r, double h, GLfloat wsp[3]);
	~Roller();
	void draw();
};