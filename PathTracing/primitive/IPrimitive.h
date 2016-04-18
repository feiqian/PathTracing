#ifndef __IPRIMITIVE_H__
#define __IPRIMITIVE_H__
#include <cmath>
#include "../common/VMath.h"
#include "../common/Material.h"
#include "../common/IntersectResult.h"
#include "../raytracer/Ray.h"

struct IntersectResult;
class IPrimitive
{
public:
	virtual IntersectResult intersect(Ray& ray) = 0;
	Ray getTransformRay(Ray& ray);
	Vec3 getTransformNormal(Vec3 normal);
	void translate(double x,double y,double z);
	void rotate(double angle, const Vec3& axis);
	void scale(double x,double y,double z);

	Material attr;
protected:
	Mat4 modelMatrixInverse,modelMatrix;
};

#endif