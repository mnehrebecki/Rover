#pragma once
#include <Windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>
#include "Wheel.h"
#include "Box.h"
#include "Lamp.h"
#include "Roller.h"

class Rover
{
public:
	Rover();
	~Rover();

	void draw();

};

