#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "Plane.h"

struct Tri : Plane
{
	Tri(Point3 origin=Point3(0,0,0),Vec3 dx = Vec3(1,0,0),Vec3 dy = Vec3(0,1,0)):Plane(origin,dx,dy){}
	IntersectResult intersect(Ray& ray);
};

#endif