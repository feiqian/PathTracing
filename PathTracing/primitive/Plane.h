#ifndef __PLANE_H__
#define __PLANE_H__
#include "IPrimitive.h"

class Plane : public IPrimitive
{
public:
	Plane(Point3 origin,Vec3 dx,Vec3 dy);
	bool intersect(Ray& ray,IntersectResult& result);
	AABB getAABB();
protected:
	//ƽ��Ĳ�������Ϊ��p(s,t) = origin + s*dx + t*dy 0<=s,t<=1
	Point3 origin;
	Vec3 dx,dy;
	Vec3 normal;
};

#endif