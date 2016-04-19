#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "Plane.h"

struct Triangle : Plane
{
	Triangle(Point3 pt1,Point3 pt2,Point3 pt3):Plane(pt1,Normalize(pt2-pt1),Normalize(pt3-pt1)){}

	IntersectResult intersect(Ray& ray);
};

#endif