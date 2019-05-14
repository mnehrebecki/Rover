// Gl_template.c
//Wy��czanie b��d�w przed "fopen"
#define  _CRT_SECURE_NO_WARNINGS



// �adowanie bibliotek:

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif




// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "../resource.h"           // About box resource identifiers.
#include "Wheel.h"
#include "Box.h"
#include "Lamp.h"
#include "Roller.h"
#include "Rover.h"

#include "object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

static GLsizei lastHeight;
static GLsizei lastWidth;

unsigned int dust = 0;

unsigned int LoadTexture(const char* file, GLenum textureSlot)
{
	GLuint texHandle;
	// Copy file to OpenGL
	glGenTextures(textureSlot, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	const auto data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, nrChannels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		//error
	}
	stbi_image_free(data);
	return texHandle;
}

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag��wek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury


// Declaration for Window procedure
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);



// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector		
	length = (float)sqrt((vector[0] * vector[0]) +
		(vector[1] * vector[1]) +
		(vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 350.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);
}

void skrzynka(void)
{
	glColor3d(0.8, 0.7, 0.3);


	glEnable(GL_TEXTURE_2D); // W��cz teksturowanie

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(0.0, 1.0); glVertex3d(-25, 25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(-25, -25, 25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, -25, 25);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(0.0, 1.0); glVertex3d(25, -25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(25, -25, -25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, 25, -25);
	glEnd();

	glDisable(GL_TEXTURE_2D); // Wy��cz teksturowanie



	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	glVertex3d(25, 25, -25);
	glVertex3d(25, -25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, 25, -25);

	glNormal3d(-1, 0, 0);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, 1, 0);
	glVertex3d(25, 25, 25);
	glVertex3d(25, 25, -25);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, -1, 0);
	glVertex3d(25, -25, 25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, -25, -25);
	glVertex3d(25, -25, -25);
	glEnd();
}


// LoadBitmapFile
// opis: �aduje map� bitow� z pliku i zwraca jej adres.
//       Wype�nia struktur� nag��wka.
//	 Nie obs�uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wska�nik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag��wek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk�adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag��wek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag��wek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wska�nik pozycji pliku na pocz�tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pami�� buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda�o si� przydzieli� pami��
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta�y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk�adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wska�nik bufora zawieraj�cego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}





void wheel(double r, double h, GLfloat srodek[])
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

void walec_prawo(double r, double h, GLfloat srodek[])
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

void walec_pion(double r, double h, GLfloat srodek[])
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

void prostopadloscian(double szer, double wys, double gleb, GLfloat poczatek[])
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
	//ty�
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

// Called to draw scene

void lazik()
{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLfloat srodek[3] = { -50.0f,15.0f,40.0f };		//wsp �rodk�w k�
		GLfloat srodek2[3] = { 50.0f,15.0f,40.0f };
		GLfloat srodek3[3] = { -50.0f,15.0f,-40.0f };
		GLfloat srodek4[3] = { 50.0f,15.0f, -40.0f };

	
		wheel(15, 15, srodek);							//ko�a
		wheel(15, 15, srodek2);
		wheel(15, 15, srodek3);
		wheel(15, 15, srodek4);

		GLfloat poczatek[3] = { -55.0f,35.0f, -20.0f };	//wsp pocz�tku prostok�t�w
		prostopadloscian(10, 5, 55, poczatek);			//o� tylna

		GLfloat poczatek1[3] = { -55.0f,10.0f, -25.0f };
		prostopadloscian(10, 30, 5, poczatek1);			//��czenie k� ty�

		GLfloat poczatek2[3] = { -55.0f,10.0f, 35.0f };
		prostopadloscian(10, 30, 5, poczatek2);


		GLfloat poczatek3[3] = { 45.0f,35.0f, -20.0f };
		prostopadloscian(10, 5, 55, poczatek3);			//o� przednia

		GLfloat poczatek4[3] = { 45.0f,10.0f, -25.0f };
		prostopadloscian(10, 30, 5, poczatek4);			//��czenie k� ty�

		GLfloat poczatek5[3] = { 45.0f,10.0f, 35.0f };
		prostopadloscian(10, 30, 5, poczatek5);

		GLfloat poczatek6[3] = { -60.0f,40.0f, -20.0f };
		prostopadloscian(120, 10, 55, poczatek6);		//pod�oga

		GLfloat srodek5[3] = { 60.0f,45.0f, -15.0f };
		walec_prawo(5, 5, srodek5);						//�wiat�o prawe

		GLfloat srodek6[3] = { 60.0f,45.0f, 7.5f };
		walec_prawo(5, 5, srodek6);						//�wiat�o �rodkowe

		GLfloat srodek7[3] = { 60.0f,45.0f, 30.0f };
		walec_prawo(5, 5, srodek7);						//�wiat�o lewe

		GLfloat srodek8[3] = { -35.0f,50.0f, 7.5f };
		walec_pion(3, 55, srodek8);						//dr��ek kamery

		GLfloat poczatek7[3] = { -38.0f, 102.0f, 10.5f };
		prostopadloscian(6, 10, 2, poczatek7);			//mocowanie kamery

		GLfloat poczatek8[3] = { -38.0f, 102.0f, 2.5f };
		prostopadloscian(6, 10, 2, poczatek8);

		GLfloat poczatek9[3] = { -41.0f, 107.0f, 4.5f };
		prostopadloscian(12, 6, 6, poczatek9);			//kamera

		GLfloat srodek9[3] = { -29.0f,111.0f, 7.5f };
		walec_prawo(2, 2, srodek9);						//obiektyw kamery

		GLfloat srodek_antena[3] = { -50.0f,50.0f, 25.0f };
		walec_pion(0.35f, 80, srodek_antena);


		GLfloat srodek_ramie[3] = { 50.0f,50.0f, 7.5f };
		walec_pion(3, 30, srodek_ramie);						//dr��ek ramienia

		GLfloat poczatek10[3] = { 47.0f, 77.0f, 10.5f };
		prostopadloscian(6, 10, 2, poczatek10);			//mocowanie ramienia

		GLfloat poczatek11[3] = { 47.0f, 77.0f, 2.5f };
		prostopadloscian(6, 10, 2, poczatek11);

		GLfloat srodek_ramie2[3] = { 47.0f, 85.0f, 7.5f };
		walec_prawo(3, 40, srodek_ramie2);

		GLfloat srodek_ramie3[3] = { 91.0f, 85.0f, 7.5f };
		walec_prawo(3, 40, srodek_ramie3);

		GLfloat poczatek12[3] = { 84.0f, 82.0f, 10.5f };
		prostopadloscian(10, 6, 2, poczatek12);			//mocowanie ramienia

		GLfloat poczatek13[3] = { 84.0f, 82.0f, 2.5f };
		prostopadloscian(10, 6, 2, poczatek13);

		GLfloat poczatek14[3] = { 128.0f, 82.0f, 10.5f };
		prostopadloscian(10, 6, 2, poczatek14);			//mocowanie ramienia

		GLfloat poczatek15[3] = { 128.0f, 82.0f, 2.5f };
		prostopadloscian(10, 6, 2, poczatek15);
}



Rover rover;
GLfloat rot[] = { 90,1,0,0 };
GLfloat pos1[3] = { 0,0,-5 };
GLfloat color1[3] = { 0.9,0.49,0.07 };

auto terrain = new object{ dust, "mars.obj", color1, pos1, rot, 10 };
void RenderScene(void)
{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////
	
	//Spos�b na odr�nienie "przedniej" i "tylniej" �ciany wielok�ta:
	//glPolygonMode(GL_BACK, GL_LINE);
	
	//teren();
	rover.draw();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dust);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	terrain->draw();
	//lazik();
	//Uzyskanie siatki:
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//Wyrysowanie prostokata:
	//glRectd(-10.0,-10.0,20.0,20.0);

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();
}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange, GreenRange, BlueRange;
	// Range for each color entry (7,7,and 3)


// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if (!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;

	// Loop through all the palette entries
	for (i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}


	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}


// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;

	// Register the window class
	if (RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
		lpszAppName,
		lpszAppName,

		// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

		// Window position and size
		50, 50,
		400, 400,
		NULL,
		NULL,
		hInstance,
		NULL);

	// If window was not created, quit
	if (hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam)
{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);

		// Select the pixel format
		SetDCPixelFormat(hDC);

		// Create palette if needed
		hPalette = GetOpenGLPalette(hDC);

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

		// �aduje pierwszy obraz tekstury:
		//bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

		glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// �aduje drugi obraz tekstury:
		//bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// ustalenie sposobu mieszania tekstury z t�em
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);

		// Tell the application to terminate after the window
		// is gone.
		PostQuitMessage(0);
		break;

		// Window is resized.
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT:
	{
		// Call OpenGL drawing code
		RenderScene();

		SwapBuffers(hDC);

		// Validate the newly painted client area
		ValidateRect(hWnd, NULL);
	}
	break;

	// Windows is telling the application that it may modify
	// the system palette.  This message in essance asks the 
	// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette)
		{
			int nRet;

			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);

			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		}
		break;


		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd))
		{
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries to system palette
			RealizePalette(hDC);

			// Remap the current colors to the newly realized palette
			UpdateColors(hDC);
			return 0;
		}
		break;

		// Key press, check for arrow keys to do cube rotation.
	case WM_KEYDOWN:
	{
		if (wParam == VK_UP)
			xRot -= 5.0f;

		if (wParam == VK_DOWN)
			xRot += 5.0f;

		if (wParam == VK_LEFT)
			yRot -= 5.0f;

		if (wParam == VK_RIGHT)
			yRot += 5.0f;

		if (wParam == 'Q')
			zRot -= 5.0f;

		if (wParam == 'E')
			zRot += 5.0f;

		xRot = (const int)xRot % 360;
		yRot = (const int)yRot % 360;
		zRot = (const int)zRot % 360;

		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	// A menu command
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// Exit the program
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;

			// Display the about box
		/*case ID_HELP_ABOUT:
			DialogBox(hInstance,
				MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
				hWnd,
				AboutDlgProc);
			break;*/
		}
	}
	break;


	default:   // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, wParam, lParam));

	}

	return (0L);
}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{

	switch (message)
	{
		// Initialize the dialog box
	case WM_INITDIALOG:
	{
		int i;
		GLenum glError;

		//// glGetString demo
		//SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, glGetString(GL_VENDOR));
		//SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, glGetString(GL_RENDERER));
		//SetDlgItemText(hDlg, IDC_OPENGL_VERSION, glGetString(GL_VERSION));
		//SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, glGetString(GL_EXTENSIONS));

		//// gluGetString demo
		//SetDlgItemText(hDlg, IDC_GLU_VERSION, gluGetString(GLU_VERSION));
		//SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, gluGetString(GLU_EXTENSIONS));


		// Display any recent error messages
		i = 0;
		do {
			glError = glGetError();
			//SetDlgItemText(hDlg, IDC_ERROR1 + i, gluErrorString(glError));
			i++;
		} while (i < 6 && glError != GL_NO_ERROR);


		return (TRUE);
	}
	break;

	// Process command messages
	case WM_COMMAND:
	{
		// Validate and Make the changes
		if (LOWORD(wParam) == IDOK)
			EndDialog(hDlg, TRUE);
	}
	break;

	// Closed from sysbox
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	}

	return FALSE;
}
