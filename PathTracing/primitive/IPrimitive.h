#ifndef __IPRIMITIVE_H__
#define __IPRIMITIVE_H__
#include<iostream>
#include <cmath>
#include "../common/VMath.h"
#include "../common/Material.h"
#include "../common/IntersectResult.h"
#include "../raytracer/Ray.h"
#include "../raytracer/AABB.h"

using namespace std;

struct IntersectResult;
class AABB;

class IPrimitive
{
public:
	virtual bool intersect(Ray& ray,IntersectResult& result) = 0;
	virtual AABB getAABB() = 0;
	virtual Point2 getTextureCoordinate(const Point3& point);

	Ray getTransformRay(Ray& ray);
	Vec3 getTransformNormal(Vec3 normal);
	void translate(double x,double y,double z);
	void rotate(double angle, const Vec3& axis);
	void scale(double x,double y,double z);
	Reflectance getReflectance(const Vec3& point);

	Material attr;
protected:
	Mat4 modelMatrixInverse,modelMatrix;
};

#endif