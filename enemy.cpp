#include "enemy.h"

Enemy::Enemy(Vec3Df pos, Vec3Df hp) : Concrete(pos)
{
	mesh.loadMesh("ufo.obj");
	mesh.scale(0.5);
	float initial_orientation = compute_orientation(hp);
	if (initial_orientation > 90 || initial_orientation < -90) {
		flipped = true;
	}
	zRotation = initial_orientation;
	computeBoundingBox();
	mesh.meshColor = Vec3Df(0.75, 0.75, 0.75);
};


/// Returns three co-ordinates which are the increments in each direction
Vec3Df Enemy::enemy_direction(Vec3Df hp)
{
	Vec3Df enemy_move;
	enemy_move[0] = (hp[0] - p[0]) / enemy_speed;
	enemy_move[1] = (hp[1] - p[1]) / enemy_speed;
	enemy_move[2] = 0;
	return enemy_move;
};

float Enemy::compute_orientation(Vec3Df hp)
{   
    /////*** writes direction from [-180, -157.5, .... 157.5 ] - 16 nos - to zRotation
    // hp = position of hero
    // p = position of enemy
    
    return round(atan2(hp[1] - p[1], hp[0] - p[0])*no_shoot_dir / 2 / PI) * 360 / no_shoot_dir;

};

/*
 * This function compute the new position for the enemy accordingly to 
 * the current position of the hero (hp).
 * @param hp the position of the hero is provided
 */
void Enemy::update(Vec3Df hp)
{
	//update position...
	Vec3Df update = enemy_direction(hp);
	p += update;
	//...and orientation
	float computed_orientation = compute_orientation(hp);
	zRotate(computed_orientation);
};

/*Bullet Enemy::shoot()
{
	Bullet b;
	return b;
};
*/
