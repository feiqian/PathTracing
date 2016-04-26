#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "IPrimitive.h"

class Sphere : public IPrimitive
{
public:
	Sphere(Point3 origin = Vec3(0,0,0),double radius = 1.0);
	bool intersect(Ray& ray,IntersectResult& result);
	Point2 getTextureCoordinate(const Point3& point);
	AABB getAABB();
protected:
	Point3 origin;
	double radius;
};

#endif