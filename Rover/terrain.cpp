#include "terrain.h"



terrain::terrain()
{
	glPushMatrix();

	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);

	if (floor.LoadFile("mars.obj"))
	{
		for (int i = 0; i < floor.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = floor.LoadedMeshes[i];

			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				glBegin(GL_TRIANGLES);
				//double random = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.55f - 0.5f)));
				glColor3f(0.55f, 0.22f, 0.07f);
				//if (i == 0) glColor3f(0, 0, 1);
				glVertex3f(
					curMesh.Vertices[curMesh.Indices[j]].Position.X,
					curMesh.Vertices[curMesh.Indices[j]].Position.Y,
					curMesh.Vertices[curMesh.Indices[j]].Position.Z
				);

				glVertex3f(
					curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
					curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
					curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z
				);

				glVertex3f(
					curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
					curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
					curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z
				);
				glEnd();
			}
		}
	}

	glPopMatrix();
}




terrain::~terrain()
{
}
