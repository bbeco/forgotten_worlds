#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "concrete.h"
#include "Vec3D.h"
#include <iostream>
#include <GL/glut.h>

class Enemy: public Concrete
{
public:
	Enemy(Vec3Df pos = Vec3Df(0,0,0)) : Concrete(pos)
	{
		mesh.loadMesh("gargoyle.obj");
		mesh.scale(0.5);
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
