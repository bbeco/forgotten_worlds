#include "util.h"

void drawPoint(Vec3Df p, Vec3Df color)
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glPointSize(40);
	glColor3f(color[0], color[1], color[2]);	
	glBegin(GL_POINTS);
	glVertex3f(p[0], p[1], p[2]);
	glEnd();
	glEnable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glPopMatrix();
}

//function to draw coordinate axes with a certain length (1 as a default)
void drawCoordSystem(float length)
{
	//draw simply colored axes
	
	//remember all states of the GPU
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//deactivate the lighting state
	glDisable(GL_LIGHTING);
	//draw axes
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(length,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,length,0);

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,length);
	glEnd();
	
	//reset to previous state
	glPopAttrib();
}
