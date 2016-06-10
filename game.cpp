#include "game.h"

void Game::drawLight()
{	
	//remember all states of the GPU
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//deactivate the lighting state
	glDisable(GL_LIGHTING);
	//yellow sphere at light position
	//glColor3f(1,1,0);
	glPushMatrix();
	//glRotatef(-45,1,0,0);
	glTranslatef(LightPos[0], LightPos[1], LightPos[2]);
	glutSolidSphere(0.1,6,6);
	glPopMatrix();

	//reset to previous state
	glPopAttrib();
}

Game::Game()
{
	LightPos[0] = 0;
	LightPos[1] = 0;
	LightPos[2] = 0;
	LightPos[3] = 1;
	red = Vec3Df(1, 0, 0);
	green = Vec3Df(0, 1, 0);
	blue = Vec3Df(0, 0, 1);
	enemies.push_back(Enemy(Vec3Df(0, 0, -3)));
	enemies.push_back(Enemy(Vec3Df(1, 0, -3)));
	hero = Hero(Vec3Df(1, 1, -3));
	activateBoss = false;
	drawArm = false;
	boss = Boss(Vec3Df(4, 0, -3));
};

void Game::init() {};

void Game::setScreenOrigin(Vec3Df origin)
{
	screenOrigin = origin;
};

void Game::display(void)
{
	glPushMatrix();
	glTranslatef(screenOrigin[0], screenOrigin[1], screenOrigin[2]);
	glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
	drawLight();
	
	//Collision detection
	vector<Bullet>::iterator ib = bullets.begin();
	vector<Enemy>::iterator ie;
	bool hit = false;
	while (ib != bullets.end() && !enemies.empty()) {
		ie = enemies.begin();
		while ( ie != enemies.end()) {
			if (ib->isHit(*ie)) {
				hit = true;
				ib = bullets.erase(ib);
				enemies.erase(ie);
				break;
			} else {
				ie++;
			}
		}
		if (!hit) {
			ib++;
		} else {
			hit = false;
		}
	}
	ie = enemies.begin();
	while (ie != enemies.end()) {
		if (hero.isHit(*ie)) {
			hero.drawBoundingBox(red);
			ie = enemies.erase(ie);
		} else {
			ie++;
			hero.drawBoundingBox();
		}
	}
	for (unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i].draw();
		enemies[i].drawBoundingBox();
	}
	hero.draw();
	for(unsigned int i=0;i<bullets.size();i++) {
		bullets[i].draw();
	}
	
	if (activateBoss) {
		boss.draw();
		if (boss.isHit(hero)) {
			boss.drawBoundingBox(red);
		} else {
			boss.drawBoundingBox();
		}
		if (drawArm) {
			boss.draw_boss_hands();
			for (int i = 0; i < boss.boss_hand_size*boss.boss_hand_num; i += 1)
			{
				if (boss.hands[i]->isHit(hero)) {
					boss.hands[i]->drawBoundingBox(red);
				} else {
					boss.hands[i]->drawBoundingBox();
				}
			}
		}
	}
	
	glPopMatrix();
};
