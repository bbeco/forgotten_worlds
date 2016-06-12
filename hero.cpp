#include "hero.h"

Hero::Hero(Vec3Df pos) : Concrete(pos)
{
	mesh.loadMesh("guard.obj");
	mesh.scale(0.5);
	computeBoundingBox();
	zRotation = 0;
	mesh.meshColor = Vec3Df(0.5, 0.97, 1);
};

Bullet Hero::shoot()
{
	Bullet b(Vec3Df(p[0]+0.5,p[1]+0.5,p[2]),zRotation, Vec3Df(1,1,0));
	return b;
}

