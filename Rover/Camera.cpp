#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::update(WPARAM wParam)
{
	switch (wParam) {
	case 'Z':
		this->position[2] -= stepMovement;
		break;

	case 'X':
		this->position[2] += stepMovement;
		break;

	case 'W':
		this->position[0] += stepMovement * sin(yaw) * cos(pitch);
		this->position[1] += stepMovement * cos(yaw) * cos(pitch);
		this->position[2] += stepMovement * sin(pitch);
		break;

	case 'S':
		this->position[0] -= stepMovement * sin(yaw) * cos(pitch);
		this->position[1] -= stepMovement * cos(yaw) * cos(pitch);
		this->position[2] -= stepMovement * sin(pitch);
		break;

	case VK_UP:
		pitch += stepRotation;
		if (pitch >= M_PI / 2) 
			pitch = M_PI / 2;
		this->lookAt[0] = sin(yaw) * cos(pitch);
		this->lookAt[1] = cos(yaw) * cos(pitch);
		this->lookAt[2] = sin(pitch);
		break;

	case VK_DOWN:
		pitch -= stepRotation;
		if (pitch <= -M_PI / 2) 
			pitch = -M_PI / 2;
		this->lookAt[0] = sin(yaw) * cos(pitch);
		this->lookAt[1] = cos(yaw) * cos(pitch);
		this->lookAt[2] = sin(pitch);
		break;

	case 'A':
		this->position[0] += stepMovement * sin(yaw - M_PI / 2);
		this->position[1] += stepMovement * cos(yaw - M_PI / 2);
		break;

	case 'D':
		this->position[0] += stepMovement * sin(yaw + M_PI / 2);
		this->position[1] += stepMovement * cos(yaw + M_PI / 2);
		break;

	case VK_RIGHT:
		yaw = (yaw - stepRotation);
		yaw = yaw - round(yaw / (2 * M_PI)) * (2 * M_PI);
		this->lookAt[0] = sin(yaw) * cos(pitch);
		this->lookAt[1] = cos(yaw) * cos(pitch);
		this->lookAt[2] = sin(pitch);
		break;

	case VK_LEFT:
		yaw = yaw + stepRotation;		
		yaw = yaw - round(yaw / (2 * M_PI)) * (2 * M_PI);
		this->lookAt[0] =  sin(yaw) * cos(pitch);
		this->lookAt[1] =  cos(yaw) * cos(pitch);
		this->lookAt[2] =  (pitch);
		break;

	default:
		break;
	}

	glLoadIdentity();
	gluLookAt(this->position[0], this->position[2], this->position[1], this->position[0] + this->lookAt[0], this->position[2] + this->lookAt[2], this->position[1] + this->lookAt[1], this->up[0], this->up[2], this->up[1]);
}

void Camera::setPosition(GLfloat position[3])
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
}
