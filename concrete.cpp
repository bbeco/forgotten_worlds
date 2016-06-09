#include <GL/glut.h>
#include "concrete.h"
#include "rotation.hpp"
#include "util.h"


/**
 * The position provided must refer to the left bottom corner of the BB of 
 * this mesh.
 * @constructor
 * @param pos The position of the BB of the mesh (it is also the position where
 * the drawing of this mesh starts)
 */
Concrete::Concrete(Vec3Df pos = Vec3Df(0,0,0))
{
	p = pos;
	zRotation = 0;
	flipped = false;
};

void Concrete::computeBoundingBox(void)
{
	Vec3Df min, max;
	Vec3Df tmp; //temporary vector to store rotated Vertex
	tmp = mesh.vertices[0].p;
	tmp[0] = (flipped) ? -tmp[0] : tmp[0];
	if (!flipped ) {
		rotateZ(zRotation, tmp);
	} else {
		rotateZ(-zRotation, tmp);
	}	
	min = max = tmp;
	for (unsigned int i = 1; i < mesh.vertices.size(); i++) {
		tmp = mesh.vertices[i].p;
		if (!flipped ) {
			rotateZ(zRotation, tmp);
		} else {
			rotateZ(-zRotation, tmp);
		}
		tmp[0] = (flipped) ? -tmp[0] : tmp[0];
		for (int j = 0; j < 3; j++) {
			if (tmp[j] < min[j]) {
				min[j] = tmp[j];
			}
			if (tmp[j] > max[j]) {
				max[j] = tmp[j];
			}
		}
	}
	bbOrigin = min;
	bbSize = max - min;
	
	//p[2] += bbSize[2];
	
	//This is setting the starting point for the character correctly
	//p -= bbOrigin; //XXX probably p should remain constant and the translation before drawing the mesh should be equal to -bbOrigin
}

void Concrete::drawBoundingBox(Vec3Df color)
{
	Vec3Df start = p - bbOrigin;
	//start[2] -= bbSize[2];
	glPushMatrix();
	glTranslatef(start[0], start[1], start[2]);
	
	Vec3Df xInc = Vec3Df(bbSize[0], 0, 0);
	Vec3Df yInc = Vec3Df(0, bbSize[1], 0);
	Vec3Df tmp;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			tmp = bbOrigin + i*xInc + j*yInc;
			//drawPoint(tmp);
		}
	}
	
	const Vec3Df corners[8] =
    {
        Vec3Df(bbOrigin[0],bbOrigin[1],bbOrigin[2]),
        Vec3Df(bbOrigin[0] + bbSize[0],bbOrigin[1],bbOrigin[2]),
        Vec3Df(bbOrigin[0],bbOrigin[1] + bbSize[1],bbOrigin[2]),
        Vec3Df(bbOrigin[0] + bbSize[0],bbOrigin[1] + bbSize[1],bbOrigin[2]),
        Vec3Df(bbOrigin[0],bbOrigin[1],bbOrigin[2] + bbSize[2]),
        Vec3Df(bbOrigin[0] + bbSize[0],bbOrigin[1],bbOrigin[2] + bbSize[2]),
        Vec3Df(bbOrigin[0],bbOrigin[1] + bbSize[1],bbOrigin[2] + bbSize[2]),
        Vec3Df(bbOrigin[0] + bbSize[0],bbOrigin[1] + bbSize[1],bbOrigin[2] + bbSize[2])
    };


    static const unsigned short faceCorners[6][4] =
    {
        { 0,4,6,2 },
        { 5,1,3,7 },
        { 0,1,5,4 },
        { 3,2,6,7 },
        { 0,2,3,1 },
        { 6,4,5,7 }
    };

	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(1.0f);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    for (unsigned short f=0;f<6;++f)
    {
        const unsigned short* face = faceCorners[f];
        for(unsigned int v = 0; v<4; v++)
            glVertex3f(corners[face[v]][0], corners[face[v]][1], corners[face[v]][2]);

    }
    glEnd();
    glPopAttrib();
    glPopMatrix();
};

void Concrete::zRotate(float angle)
{
	zRotation = angle;
	computeBoundingBox();
};

void Concrete::flip(void)
{
	flipped = !flipped;
	computeBoundingBox();
};

/**
 * This checks if a point is inside the bounding box of this object
 * @param x a Vec3Df point that has to be checked
 */
bool Concrete::checkIfInsideBoundingBox(Vec3Df x)
{
	if (p[0] <= x[0] &&
		x[0] <= p[0] + bbSize[0] &&
		p[1] <= x[1] &&
		x[1] <= p[1] + bbSize[1]) {
			return true;
		}
	return false;
}

/**
 * Collision detection
 */
bool Concrete::isHit(Concrete & c)
{
	if (c.p[0] <= (p + bbSize)[0] &&
		(c.p + c.bbSize)[0] >= p[0] &&
		c.p[1] <= (p + bbSize)[1] &&
		(c.p + c.bbSize)[1] >= p[1]) {
			return true;
	}	
	return false;
};

/*
 * Debugging purposes
 */
void Concrete::drawOrigin(Vec3Df color)
{
	Vec3Df start = p - bbOrigin;
	glPushMatrix();
	glTranslatef(start[0], start[1], start[2]);
	glDisable(GL_LIGHTING);
	glPointSize(40);
	glColor3f(color[0], color[1], color[2]);	
	glBegin(GL_POINTS);
	glVertex3f(bbOrigin[0], bbOrigin[1], bbOrigin[2]);
	glEnd();
	glEnable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glPopMatrix();
};

void Concrete::draw() {
	drawPoint(p);
	Vec3Df start = p - bbOrigin;
	glPushMatrix();
	glTranslatef(start[0], start[1], start[2]);
	if (!flipped) {
		glRotatef(zRotation, 0, 0, 1);
	} else {
		glRotatef(180, 0, 1, 0);
		glRotatef(-zRotation, 0, 0, 1);
	}
	mesh.draw();
	glPopMatrix();
};
