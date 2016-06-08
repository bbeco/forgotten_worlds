#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include "enemy.h"
#include "hero.h"
#include "util.h"

using namespace std;

class Game
{
private:
	Vec3Df screenOrigin;
	vector<Enemy> enemies;
	Hero hero;
	
	float LightPos[4];
	Vec3Df red;
	Vec3Df green;
	Vec3Df blue;
	
	void drawLight();
public:
	Game();
	void setScreenOrigin(Vec3Df o);
	void init();
	void display();
};

#endif
