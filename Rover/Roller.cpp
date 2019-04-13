#include "Roller.h"



Roller::Roller(double r, double h, GLfloat wsp[3]) :r(r), h(h)
{

	srodek[0] = wsp[0];
	srodek[1] = wsp[1];
	srodek[2] = wsp[2];
}


Roller::~Roller()
{
}

void Roller::draw()
{
	double x, y, z, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.216, 0.210, 0.210);
	glVertex3fv(srodek);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(x, srodek[1], z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(0.216, 0.210, 0.210);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(x, srodek[1], z);
		glVertex3d(x, h + srodek[1], z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.216, 0.210, 0.210);
	srodek[1] += h;
	glVertex3fv(srodek);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		z = srodek[2] + r * cos(alpha);
		glVertex3d(x, srodek[1], z);
	}
	glEnd();
}