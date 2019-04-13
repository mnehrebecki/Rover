#include "Lamp.h"



Lamp::Lamp(double r, double h, GLfloat wsp[3]) :r(r), h(h)
{

	srodek[0] = wsp[0];
	srodek[1] = wsp[1];
	srodek[2] = wsp[2];
}


Lamp::~Lamp()
{
}
void Lamp::draw()
{
	double x, y, z, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1, 1, 0);
	glVertex3fv(srodek);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y = srodek[1] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(srodek[0], y, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(0.0, 0.0, 0);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y = srodek[1] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(srodek[0], y, z);
		glVertex3d(h + srodek[0], y, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1, 1, 0);
	srodek[0] += h;
	glVertex3fv(srodek);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		y = srodek[1] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(srodek[0], y, z);
	}
	glEnd();
}