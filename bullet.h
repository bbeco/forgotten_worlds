#ifndef _BULLET_H_
#define _BULLET_H_

#include "Vec3D.h"
#include "concrete.h"
#include <iostream>
#include <GL/glut.h>

class Bullet: public Concrete
{
protected:
	float size;
	Vec3Df speed;
	GLuint text[2];
	Vec3Df col;
public:
	bool mytext;
	Bullet (Vec3Df pos = Vec3Df(0,0,0),float theta = 0,Vec3Df col = Vec3Df(1,1,0));
	void computeBoundingBox();
	void draw();
	void update();

};
#endif
