#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "concrete.h"
#include "Vec3D.h"
#include <iostream>
#include <GL/glut.h>

class Enemy: public Concrete
{
public:
	Enemy(Vec3Df pos = 0) : Concrete(pos)
	{
		mesh.loadMesh("gargoyle.obj");
		//mesh.rotate(90, 1, 0, 0);
		computeBoundingBox();
	};
	void zRotate(float angle)
	{
		Concrete::zRotate(angle);
	};
	void flip()
	{
		Concrete::flip();
	};
};

#endif
