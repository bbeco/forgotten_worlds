#include "bullet.h"
#include <cmath>
#include "loadppm.h"

Vec3Df computeSpeed(float zRotation){
	switch ((int)zRotation) {
	case 0:
		return Vec3Df(0.4,0,0);
	case 90:
		return Vec3Df(0,-0.4,0);
	case -90:
		return Vec3Df(0,0.4,0);
	default:
		float param = -zRotation*M_PI/180;
		float c, s;
		c = cos(param);
		s = sin(param);
		return Vec3Df(0.4*c,0.4*s,0); 
	}
}
Bullet::Bullet(Vec3Df pos, float theta): Concrete(pos)
{
	size = 0.1;
	computeBoundingBox();
	zRotation = theta;
	speed = computeSpeed(zRotation);
	if(flipped){
		speed[0] = -speed[0];
	}
	PPMImage image3("bullet.ppm");
	glGenTextures(1, &text[0]);
	glBindTexture(GL_TEXTURE_2D, text[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image3.sizeX, image3.sizeY, 
		GL_RGB, GL_UNSIGNED_BYTE, image3.data);
		
	PPMImage image2("sand.ppm");
	glGenTextures(1, &text[1]);
	glBindTexture(GL_TEXTURE_2D, text[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image2.sizeX, image2.sizeY, 
		GL_RGB, GL_UNSIGNED_BYTE, image2.data);
	mytext = true;
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
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(red, green, blue);
	glNormal3f(0,0,-1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glBegin(GL_QUADS);
		
		glTexCoord2f(0,0);
		glVertex3f(0, 0, 0);
		
		glTexCoord2f(0,1);
		glVertex3f(0, size, 0);
		
		glTexCoord2f(1,1);
		glVertex3f(size, size, 0);
		
		glTexCoord2f(1,0);
		glVertex3f(size, 0, 0);
	glEnd();
	glPopAttrib();
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
	glEnable(GL_TEXTURE_2D);
	if(mytext == false){
		glBindTexture(GL_TEXTURE_2D, text[0]);
	} else {
		glBindTexture(GL_TEXTURE_2D, text[1]);
	}
	drawUnitCube(0,1,1,size);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void Bullet::update()
{
	p += speed;
	//mytext = !mytext;
}
