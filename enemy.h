#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "concrete.h"
#include "mesh.h"
#include "Vec3D.h"
#include <iostream>
#include <GL/glut.h>

class Enemy: public Concrete
{
protected:
	Mesh mesh;
public:
	Enemy(Vec3Df pos = 0) : Concrete(pos), mesh()
	{
		mesh.loadMesh("gargoyle.obj");
		mesh.computeBoundingBox();
	}
	void draw()
	{
		glPushMatrix();
		glTranslatef(p[0], p[1], p[2]);
		glRotatef(90, 1, 0, 0);
		mesh.draw();
		glPopMatrix();
	}
	void drawBoundingBox(Vec3Df color = Vec3Df(0, 1, 0))
	{
		glPushMatrix();
		glTranslatef(p[0], p[1], p[2]);
		glRotatef(90, 1, 0, 0);
		//std::cout << "enemy p = " << p << std::endl;
		mesh.drawBoundingBox(color);
		glPopMatrix();
	}
	bool isHit(Concrete & other) { return true; };
};

#endif
