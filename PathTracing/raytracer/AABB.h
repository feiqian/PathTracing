#ifndef __AABB_H__
#define __AABB_H__
#include <iostream>
#include "../common/IntersectResult.h"
#include "../raytracer/Ray.h"

using namespace std;

class AABB
{
public:
	AABB(){};
	AABB(Point3 low,Point3 high);

	bool intersect(Ray& ray,IntersectResult& result);
	AABB getAABB(){return *this;}

	inline double minCoordinate(int axis){return low.num[axis];}
	inline double maxCoordinate(int axis){return high.num[axis];}

	static AABB combine(const AABB& box1,const AABB& box2);
private:
	Point3 low,high;
};

#endif