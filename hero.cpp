#include "hero.h"

Hero::Hero(Vec3Df pos) : Concrete(pos)
{
	mesh.loadMesh("Guard.obj");
	mesh.scale(0.5);
	computeBoundingBox();
};

Bullet Hero::shoot()
{
	Bullet b(Vec3Df(p[0]+0.5,p[1]+0.5,p[2]+0.3),Vec3Df(0.4,0,0));
	return b;
}

