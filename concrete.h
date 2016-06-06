#ifndef _CONCRETE_H_
#define _CONCRETE_H_

#include "mesh.h"
#include "Vec3D.h"

class Concrete
{
protected:
	float zRotation;
	bool flipped; //true if the mesh has been flipped around y-axe
	Mesh mesh;
	//Bounding box information
	//point of bounding box with minimal coordinates ("lower left corner")
	Vec3Df bbOrigin, bbSize;
	
	void drawCell(const Vec3Df & Min,const Vec3Df& Max);
	void computeBoundingBox(void);
	bool checkIfInsideBoundingBox(Vec3Df);
public:	
	Vec3Df p; //object position in the scene
	Concrete(Vec3Df pos);
	bool isHit(Concrete & other);
	void draw();
	void drawBoundingBox(Vec3Df color = Vec3Df(0,1,0));
	void drawOrigin(Vec3Df color = Vec3Df(1,0,0));
	void zRotate(float angle);
	void flip();
};

#endif
