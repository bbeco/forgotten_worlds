#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <iostream>
#include <GL/glut.h>
#include "concrete.h"
#include "Vec3D.h"
#include "bullet.h"


class Enemy: public Concrete
{
public:
	bool activated;
	Enemy(Vec3Df pos = Vec3Df(0,0,0));
//	void update(Vec3Df hp);
	//Bullet shoot();
};


#endif
