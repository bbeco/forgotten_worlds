#include "hero.h"

Hero::Hero(Vec3Df pos) : Concrete(pos)
{
	mesh.loadMesh("guard.obj");
	mesh.scale(0.5);
	computeBoundingBox();
	zRotation = 0;
};

Bullet Hero::shoot()
{
	Bullet b(Vec3Df(p[0]+0.5,p[1]+0.5,p[2]),zRotation);
	return b;
}

