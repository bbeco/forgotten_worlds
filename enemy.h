#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "concrete.h"
#include "mesh.h"

class Enemy: public Concrete
{
protected:
	Mesh mesh;
public:
	Enemy() : mesh ()
	{
		mesh.loadMesh("gargoyle.obj");
		mesh.computeBoundingBox();
	}
	void draw()
	{
		mesh.draw();
	}
	void drawBoundingBox()
	{
		mesh.drawBoundingBox();
	}
	void isHit() {};
};

#endif
