#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <iostream>
#include <GL/glut.h>
#include "concrete.h"
#include "Vec3D.h"
#include "bullet.h"


class Enemy: public Concrete
{
protected:
	static const float enemy_speed = 100;
	static const float PI = 3.14;
	static const int no_shoot_dir = 16;
	
	Vec3Df enemy_direction(Vec3Df hp);
	float compute_orientation(Vec3Df hp);
	
public:
	Enemy(Vec3Df pos, Vec3Df hp);
	void update(Vec3Df hp);
	Bullet* shoot();
};


#endif
