#pragma once
#include <windows.h>  

#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>  
#include "OBJ_Loader.h"

class terrain
{
public:
	terrain();
	~terrain();

	objl::Loader floor;
};

