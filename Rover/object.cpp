#include "object.h"

object::object(unsigned int *texID, std::string file, GLfloat color[3], GLfloat pos[3], GLfloat rot[4], GLfloat scale) : texID(texID), scale(scale)
{
	floor.LoadFile(file);
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
	this->rot[0] = rot[0];
	this->rot[1] = rot[1];
	this->rot[2] = rot[2];
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}


object::~object()
{
}

void object::draw() {
	glPushMatrix();

	glRotatef(rot[0], rot[1], rot[2], rot[3]);
	glScalef(scale, scale, scale);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *texID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < floor.LoadedMeshes.size(); i++)
	{
		curMesh = floor.LoadedMeshes[i];

		for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			glBegin(GL_TRIANGLES);
			glColor3fv(color);
			glTexCoord2d(0, 0); glVertex3f(
				pos[0] / scale + curMesh.Vertices[curMesh.Indices[j]].Position.X,
				pos[2] / scale + curMesh.Vertices[curMesh.Indices[j]].Position.Y,
				pos[1] / scale + curMesh.Vertices[curMesh.Indices[j]].Position.Z
			);

			glTexCoord2d(0, 1); glVertex3f(
				pos[0] / scale + curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
				pos[2] / scale + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
				pos[1] / scale + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z
			);

			glTexCoord2d(1, 0); glVertex3f(
				pos[0] / scale + curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
				pos[2] / scale + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
				pos[1] / scale + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z
			);
			glEnd();
		}

	}
	//glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}