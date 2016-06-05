#include "util.h"

void drawPoint(Vec3Df p, Vec3Df color)
{
	glDisable(GL_LIGHTING);
	glPointSize(40);
	glColor3f(color[0], color[1], color[2]);	
	glBegin(GL_POINTS);
	glVertex3f(p[0], p[1], p[2]);
	glEnd();
	glEnable(GL_LIGHTING);
	glColor3f(1, 1, 1);
}
