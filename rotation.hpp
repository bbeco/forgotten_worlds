#ifndef _SIMPLE_ROTATIONS_
#define _SIMPLE_ROTATIONS_

#define _USE_MATH_DEFINES
#include <cmath>
#include "Vec3D.h"

/**
 * Compute a rotation of the given point accordingly to the angle.
 * This works with enough precision with angles between 0 and 180 degrees
 * @param angle The angle between [0; 180] in degrees.
 * @param v The point to be rotated.
 */
void rotateZ(float angle, Vec3Df & v)
{
	float x, y;
	int int_angle = (int)angle;
	switch (int_angle) {
	case 0:
		return;
	case 90:
		x = -v[1];
		y = v[0];
		break;
	case 180:
		x = -v[0];
		y = -v[1];
		break;
	default:
		float param = angle*M_PI/180;
		float c, s;
		c = cos(param);
		s = sin(param);
		x = v[0]*c - v[1]*s;
		y = v[0]*s + v[1]*c;
		//z component does not change
	}
	v[0] = x;
	v[1] = y;
}

#endif
