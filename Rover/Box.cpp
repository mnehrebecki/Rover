#include "Box.h"



Box::Box(double length, double height, double width, GLfloat wsp[]):szer(length),wys(height),gleb(width)
{
	poczatek[0] = wsp[0];
	poczatek[1] = wsp[1];
	poczatek[2] = wsp[2];
}


Box::~Box()
{
}


void Box::draw()
{
	float x = 0, y = 0, z = 0;
	int ile;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//front
	for (int i = 0; i < szer; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3d(0.300, 0.300, 0.300);

		for (int j = 0; j <= wys;j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2]);
			glVertex3d(poczatek[0] + 1.0 + x, poczatek[1] + y, poczatek[2]);

			y += 1;
		}
		x += 1;
		y = 0;
		glEnd();

	}
	//ty³
	x = 0;
	for (int i = 0; i < szer; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3d(0.300, 0.300, 0.300);

		for (int j = 0; j <= wys; j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + gleb);
			glVertex3d(poczatek[0] + 1.0 + x, poczatek[1] + y, poczatek[2] + gleb);

			y += 1;
		}
		x += 1;
		y = 0;
		glEnd();

	}

	//boki
	x = 0, y = 0, z = 0;
	for (int i = 0; i < gleb; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3d(0.230, 0.230, 0.230);

		for (int j = 0; j <= wys; j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + z);
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + 1.0 + z);

			y += 1;
		}
		y -= 1;
		for (int j = 0; j <= szer; j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + z);
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + 1.0 + z);

			x += 1;
		}
		x -= 1;
		for (int j = 0; j <= wys; j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + z);
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + 1.0 + z);

			y -= 1;
		}
		y += 1;
		for (int j = 0; j <= szer; j++)
		{
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + z);
			glVertex3d(poczatek[0] + x, poczatek[1] + y, poczatek[2] + 1.0 + z);

			x -= 1;
		}
		z += 1;

		y = 0;
		x = 0;
		glEnd();
	}

}
