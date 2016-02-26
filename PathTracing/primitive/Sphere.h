#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "IPrimitive.h"

class Sphere : public IPrimitive
{
public:
	Sphere(Point3 origin = Vec3(0,0,0),double radius = 1.0);
	IntersectResult intersect(Ray& ray);
protected:
	Point3 origin;
	double radius;
};

#endif