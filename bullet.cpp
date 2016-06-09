#include "bullet.h"

Bullet::Bullet(Vec3Df pos, Vec3Df speed): Concrete(pos)
{
	size = 0.1;
	computeBoundingBox();
	this->speed = speed;
}

void Bullet::computeBoundingBox()
{
	bbOrigin = Vec3Df(0,0,0);
	bbSize = Vec3Df(size,size,size);
}

void drawUnitFace(float red, float green, float blue,float size)
{
	//1) draw a unit quad in the x,y plane oriented along the z axis
	//2) make sure the orientation of the vertices is positive (counterclock wise)
	//3) What happens if the order is inversed?
	glColor3f(red, green, blue);
	glNormal3f(0,0,-1);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		glVertex3f(size, size, 0);
		glVertex3f(size, 0, 0);
	glEnd();
}
void drawUnitCube(float red, float green, float blue,float size)
{
	//bottom
	drawUnitFace(red, green, blue,size);
	glPushMatrix();
	
	//right
	glTranslatef(size, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue,size);
	
	//top
	glTranslatef(size, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue,size);
	
	//left
	glTranslatef(size, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue,size);
	
	glPopMatrix();
	
	//behind
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -size);
	drawUnitFace(red, green, blue,size);
	glPopMatrix();
	
	//front
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -size, 0);
	drawUnitFace(red, green, blue,size);
	glPopMatrix();
	
	

}
void Bullet::draw()
{
	Vec3Df start = p - bbOrigin;
	glPushMatrix();
	glTranslatef(start[0],start[1],start[2]);
	drawUnitCube(1,1,1,size);
	glPopMatrix();
}
void Bullet::update()
{
	p += speed;
}
