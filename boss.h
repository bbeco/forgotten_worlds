#ifndef _BOSS_H_
#define _BOSS_H_

#include "Vec3D.h"
#include "concrete.h"
#include <iostream>
#include <GL/glut.h>
#include "util.h"
#include "bullet.h"


class Hand_Boss: public Concrete
{
protected:
	float boss_hand_radius;
	int boss_hand_stacks;
public:
 
	Hand_Boss (Vec3Df pos);
	
	void computeBoundingBox();
	
	void draw(Vec3Df color);
	
	Bullet shoot();
	
};

class Boss: public Concrete
{
protected:
	float PI;
	int boss_hand_stacks;
	int boss_hand_period; // number of speheres per wave
	int boss_hand_count; // for changing hand shapes
	int boss_hand_count_max; 
	int boss_hand_count_step; // variety of hand shapes  = max/step
	bool count_up;
	float boss_hand_radius;
		
public: 
	Vec3Df bossColor;
	int boss_hand_size;
	int boss_hand_num;
	Hand_Boss** hands;
	Mesh simplifiedMesh[12];
	
	Boss(Vec3Df pos = Vec3Df(0,0,0));
	void draw(Vec3Df cameraPos, Vec3Df color);
	void create_boss_hands();
	
	void draw_boss_hands();
	
	void assignMesh(unsigned int life);
	
	void update_boss_hand_pos(bool fly);
	
};
#endif
