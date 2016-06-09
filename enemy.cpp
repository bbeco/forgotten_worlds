#include "enemy.h"

Enemy::Enemy(Vec3Df pos) : Concrete(pos)
{
	mesh.loadMesh("gargoyle.obj");
	mesh.scale(0.5);
	computeBoundingBox();
};

/*
 * This function compute the new position for the enemy accordingly to 
 * the current position of the hero (hp).
 * @param hp the position of the hero is provided
 */
/*void Enemy::update(Vec3Df hp)
{
	//the position of the enemy is in the attribute p
	p = //..
};
*/
/*Bullet Enemy::shoot()
{
	Bullet b;
	return b;
};
*/
