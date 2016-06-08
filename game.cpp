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
	//drawCoordSystem();
	drawPoint(Vec3Df(0.1, 0.1, -0.1));
	for (unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i].draw();
		//enemies[i].drawOrigin(Vec3Df(0, 0, 1));
		enemies[i].drawBoundingBox(green);
	}
	glPopMatrix();
};
