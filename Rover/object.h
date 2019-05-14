#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include "OBJ_Loader.h"
#include <string>

class object
{
public:
	object(unsigned int *texID, std::string file, GLfloat color[3], GLfloat pos[3], GLfloat rot[4], GLfloat scale);
	~object();
	void draw();
private:
	GLfloat pos[3];
	GLfloat color[3];
	GLfloat rot[4];
	GLfloat scale;
	objl::Loader floor;
	objl::Mesh curMesh;
	std::string file;
	unsigned int *texID;
};