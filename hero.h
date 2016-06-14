#ifndef _HERO_H_
#define _HERO_H_
#include "Vec3D.h"
#include "concrete.h"
#include "bullet.h"


class Hero: public Concrete
{
public:
	Hero(Vec3Df pos  = Vec3Df(0, 0, 0));
	Bullet *shoot();
};

#endif
