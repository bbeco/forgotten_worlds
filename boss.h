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
	Hand_Boss (Vec3Df pos): Concrete(pos)
	{
		boss_hand_radius = 0.1;
		boss_hand_stacks = 50;
		computeBoundingBox();
	}
	void computeBoundingBox()
	{
		bbOrigin = Vec3Df(-0.1, -0.1, -0.1);
		bbSize = Vec3Df(2*boss_hand_radius,2*boss_hand_radius,2*boss_hand_radius);
	}
	void draw()
	{		
		Vec3Df start = p - bbOrigin;
		glPushMatrix();
		glTranslatef(start[0], start[1], start[2]);
		glutSolidSphere(boss_hand_radius, boss_hand_stacks, boss_hand_stacks);	
		glPopMatrix();
	}
	
	Bullet shoot()
	{
		Bullet b(p,180,Vec3Df(1,1,1));
		return b;
	}
	
};

class Boss: public Concrete
{
private:
	float myAbs (float x)
	{
		if(x < 0){
			x = -x;
		}
		return x;
	}
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
	int boss_hand_size;
	int boss_hand_num;
	Hand_Boss** hands;
	Mesh simplifiedMesh[10];
	
	Boss(Vec3Df pos = Vec3Df(0,0,0)) : Concrete(pos)
	{
		mesh.loadMesh("Bane_3.obj");
		computeBoundingBox();
		boss_hand_size = 10;
		boss_hand_num = 2;
		PI = 3.14;
		boss_hand_stacks = 50;
		boss_hand_period = 10; 
		boss_hand_count = 1; 
		boss_hand_count_max = 2000; 
		boss_hand_count_step = 10;
		count_up = true;
		boss_hand_radius = 0.1;
		hands = new Hand_Boss*[boss_hand_num*boss_hand_size];
		for(unsigned int i = 0;i < 10;i++){
			simplifiedMesh[i] = mesh.simplifyMesh(i*2);
		}
	}
	void create_boss_hands() {
        for (int i = 0;i < boss_hand_num*boss_hand_size; i++){
			if(i%boss_hand_size == 0){
				if(i == 0){
					hands[i] = new Hand_Boss(Vec3Df(p[0],p[1]+0.5,p[2]));
				} else{
					hands[i] = new Hand_Boss(Vec3Df(p[0]+1,p[1]+0.6,p[2]+1));
				}
			} else {
				hands[i] = new Hand_Boss(Vec3Df(hands[i-1]->p[0]-(2*boss_hand_radius),hands[i-1]->p[1],hands[i-1]->p[2]));
			}
		}
	}
	void draw_boss_hands() {
		glColor3f(0, 0, 1);
		for(int i=0; i<boss_hand_num*boss_hand_size;i++){
			hands[i]->draw();
		}
		glColor3f(1,1,1);
	}
	void assignMesh(unsigned int life)
	{	
		mesh = simplifiedMesh[life];
	}
	
	void update_boss_hand_pos(bool fly) {
		for (int i = 0; i < boss_hand_num*boss_hand_size; i++) {
			if(fly == true){
				hands[i]->p[0] += 2 * boss_hand_radius * myAbs(cos((i/2)*PI*((boss_hand_count % boss_hand_count_max) / boss_hand_period) +(1-2*(i/boss_hand_size)) * PI/6))/1.41;
				hands[i]->p[1] += 2 * boss_hand_radius * myAbs(sin((i/2)*PI*((boss_hand_count % boss_hand_count_max) / boss_hand_period) +(1-2*(i/boss_hand_size))* PI/6));
			}
			if(i%boss_hand_size != 0){
				if(fly == false){
					hands[i]->p[0] = hands[i-1]->p[0] - 2 * boss_hand_radius * myAbs(cos((i/2)*PI*((boss_hand_count % boss_hand_count_max) / boss_hand_period) +(1-2*(i/boss_hand_size)) * PI/6))/1.41;
					hands[i]->p[1] = hands[i-1]->p[1] + 2 * boss_hand_radius * myAbs(sin((i/2)*PI*((boss_hand_count % boss_hand_count_max) / boss_hand_period) +(1-2*(i/boss_hand_size))* PI/6));
				}
				if (count_up) {
						boss_hand_count = boss_hand_count + boss_hand_count_step;
						if (boss_hand_count == boss_hand_count_max)
								count_up = false;
				}
				else {
						boss_hand_count = boss_hand_count - boss_hand_count_step;
						if(boss_hand_count == 1)
								count_up = true;
				}                        
			}
		}
	}

	
};
#endif
