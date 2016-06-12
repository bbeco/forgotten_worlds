#include "boss.h"

Hand_Boss::Hand_Boss (Vec3Df pos): Concrete(pos)
	{
		boss_hand_radius = 0.1;
		boss_hand_stacks = 50;
		computeBoundingBox();
	}

void Hand_Boss::computeBoundingBox()
	{
		bbOrigin = Vec3Df(-0.1, -0.1, -0.1);
		bbSize = Vec3Df(2*boss_hand_radius,2*boss_hand_radius,2*boss_hand_radius);
	}

void Hand_Boss::draw()
	{		
		Vec3Df start = p - bbOrigin;
		glPushMatrix();
		glTranslatef(start[0], start[1], start[2]);
		glutSolidSphere(boss_hand_radius, boss_hand_stacks, boss_hand_stacks);	
		glPopMatrix();
	}

Bullet Hand_Boss::shoot()
	{
		Bullet b(p,180,Vec3Df(0.53,0.8,0.9));
		return b;
	}
	
Boss::Boss(Vec3Df pos) : Concrete(pos)
	{
		mesh.loadMesh("Bane_3.obj");
		computeBoundingBox();
		boss_hand_size = 15;
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
		for(unsigned int i = 0;i < 12;i++){
			simplifiedMesh[i] = mesh.simplifyMesh(i*2);
		}
	}
	
void Boss::create_boss_hands() 
	{
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
	
void Boss::draw_boss_hands() 
	{
		glColor3f(0, 0, 1);
		for(int i=0; i<boss_hand_num*boss_hand_size;i++){
			hands[i]->draw();
		}
		glColor3f(1,1,1);
	}		

void Boss::assignMesh(unsigned int life)
	{	
		mesh = simplifiedMesh[life];
	}
	
float myAbs (float x)
	{
		if(x < 0){
			x = -x;
		}
		return x;
	}
		
void Boss::update_boss_hand_pos(bool fly) 
	{
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
