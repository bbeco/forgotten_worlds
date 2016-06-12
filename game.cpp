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
//	enemies.push_back(Enemy(Vec3Df(2, 0, -3)));
//	enemies.push_back(Enemy(Vec3Df(-2, 0, -3)));
	hero = Hero(Vec3Df(-0.5, 1, -3));
	activateBoss = false;
	drawArm = false;
	boss = Boss(Vec3Df(3.5, 0, -3));
	bossLife = 12;
	numberOfEnemies = 0;
	bossCount = 24;
	heroLife = 400;
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
	
	/*
	 * Collision detection
	 */
	//hero's bullets and enemies
	vector<Bullet>::iterator ib = bullets.begin();
	vector<Enemy*>::iterator ie;
	bool hit = false;
	cout<<"CD:hero's bullets and enemies"<<endl;
	while (ib != bullets.end() && !enemies.empty()) {
		ie = enemies.begin();
		while ( ie != enemies.end()) {
			if (ib->isHit(**ie)) {
				hit = true;
				ib = bullets.erase(ib);
				//enemies are dynamically allocated!
				delete(*ie);
				enemies.erase(ie);
				numberOfEnemies--;
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
	//hero and enemies' bullets
	cout<<"CD:hero and enemies' bullet"<<endl;
	ib = enemyBullets.begin();
	while(ib != enemyBullets.end()){
		if(ib->isHit(hero)){
			heroLife--;
			ib = enemyBullets.erase(ib);
		} else {
			ib++;	
		}
	}
	//hero and enemies
	cout<<"CD:hero and enemies"<<endl;
	ie = enemies.begin();
	while (ie != enemies.end()) {
		if (hero.isHit(**ie)) {
			//hero.drawBoundingBox(red);
			heroLife--;
			delete(*ie);
			ie = enemies.erase(ie);
			numberOfEnemies--;
		} else {
			ie++;
			//hero.drawBoundingBox();
		}
	}
	cout<<"empty enemies' bulltes vector"<<endl;
	if(enemies.size() == 0){
		
		enemyBullets.clear();
		
	}
	
	/*
	 * Drawing scene
	 */
	for (unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
		//enemies[i]->drawBoundingBox();
	}
//	if(heroLife > 0){
		hero.draw();
//	}
	
	for(unsigned int i=0;i<bullets.size();i++) {
		bullets[i].draw();
	}
	
	for(unsigned int i=0;i<enemyBullets.size();i++) {
		enemyBullets[i].draw();
	}
	cout<<"end of display, before boss"<<endl;
	if (activateBoss) {
		
		boss.draw();
		//collision detection between hero and boss
		cout<<"CD:hero and boss"<<endl;
		if (boss.isHit(hero)) {
			heroLife--;
			if (bossCount > 0) {
				bossCount --;
				if(bossCount%2!=0){
					bossLife--;
				}
			}
			boss.assignMesh(bossLife);
		}
		//collision detection between hero's bullets and boss
		
		vector<Bullet>::iterator ib = bullets.begin();
		
		while(ib != bullets.end()){
			cout<<"CD:hero's bullets and boss"<<endl;
			if(ib->isHit(boss)){
				cout<<"hit hero's bullets and boss"<<endl;
				ib = bullets.erase(ib);
				if (bossCount > 0) {
					bossCount --;
					if(bossCount%2!=0){
						bossLife--;
					}
				}
				boss.assignMesh(bossLife);
			} else {
				ib++;
			}
			
		}
		
		if (drawArm) {
			boss.draw_boss_hands();
			for(unsigned int i = 0; i < bossBullets.size();i++){
				bossBullets[i].draw();
			}
			//collision detection with boss' arms
			cout<<"CD:boss' arm"<<endl;
			for (int i = 0; i < boss.boss_hand_size*boss.boss_hand_num; i += 1)
			{
				//collision detection between hero and arms
				if (boss.hands[i]->isHit(hero)) {
					heroLife--;
				//	boss.hands[i]->drawBoundingBox(red);
				}
				//collision detection between hero's bullets and arms
				ib = bullets.begin();
				while(ib != bullets.end()){
					if(ib->isHit(*boss.hands[i])){
						ib = bullets.erase(ib);
					//	boss.hands[i]->drawBoundingBox(red);
					} else {
						ib++;
					}
				}
			}
		}
	}
	
	glPopMatrix();
};
