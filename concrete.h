#ifndef _CONCRETE_H_
#define _CONCRETE_H_

#include "mesh.h"
#include "Vec3D.h"

class Concrete
{
private:
	void drawCell(const Vec3Df & Min,const Vec3Df& Max);
	void computeBoundingBox(void);
protected:
	Mesh mesh;
	virtual void isHit(void) = 0;
};

#endif
