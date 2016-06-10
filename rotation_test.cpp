#include "rotation.hpp"
#include "iostream"
#include "Vec3D.h"

using namespace std;

int main (int argc, char const* argv[])
{
	Vec3Df v = Vec3Df(1, 1, 1);
	rotateZ(45, v);
	cout << v << endl;
	return 0;
}
