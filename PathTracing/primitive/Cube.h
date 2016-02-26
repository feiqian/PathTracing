#ifndef __CUBE_H__
#define __CUBE_H__
#include "IPrimitive.h"

class Cube : public IPrimitive
{
public:
	Cube(Point3 origin=Vec3(0,0,0),double length=1.0,double width=1.0,double height=1.0);
	IntersectResult intersect(Ray& ray);
private:
	Point3 origin;
	double length,width,height;
};

#endif