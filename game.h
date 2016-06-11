#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include "enemy.h"
#include "hero.h"
#include "util.h"
#include "boss.h"
#include "bullet.h"

using namespace std;

class Game
{
private:
	Vec3Df screenOrigin;
	vector<Enemy> enemies;
	
	float LightPos[4];
	Vec3Df red;
	Vec3Df green;
	Vec3Df blue;
	
	unsigned int bossCount;
	void drawLight();
public:
	vector<Bullet> bullets;
	vector<Bullet> bossBullets;
	Boss boss;
	Hero hero;
	bool activateBoss;
	bool drawArm;
	unsigned int bossLife;
	Game();
	void setScreenOrigin(Vec3Df o);
	void init();
	void display();
};

#endif
