#ifndef __PLANE_H__
#define __PLANE_H__
#include "IPrimitive.h"

struct Plane : IPrimitive
{
	Plane(Point3 origin=Point3(0,0,0),Vec3 dx = Vec3(1,0,0),Vec3 dy = Vec3(0,1,0));
	virtual IntersectResult intersect(Ray& ray);

	//平面的参数方程为：p(s,t) = origin + s*dx + t*dy 0<=s,t<=1
	Point3 origin;
	Vec3 dx,dy;
	Vec3 normal;
};

#endif