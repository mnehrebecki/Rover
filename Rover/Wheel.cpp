#include "Wheel.h"



Wheel::Wheel(double r, double h, GLfloat wsp[3]):r(r),h(h)
{
	
	srodek[0] = wsp[0];
	srodek[1] = wsp[1];
	srodek[2] = wsp[2];
}


Wheel::~Wheel()
{
}


void Wheel::draw()
{
	double x, y, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.300, 0.300, 0.300);
	glVertex3fv(srodek);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		y = srodek[1] + r * cos(alpha);
		glVertex3d(x, y, srodek[2]);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3d(0.0, 0.0, 0);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		y = srodek[1] + r * cos(alpha);
		glVertex3d(x, y, srodek[2]);
		glVertex3d(x, y, h + srodek[2]);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.300, 0.300, 0.300);
	srodek[2] += h;
	glVertex3fv(srodek);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		x = srodek[0] + r * sin(alpha);
		y = srodek[1] + r * cos(alpha);
		glVertex3d(x, y, srodek[2]);
	}
	glEnd();
}
