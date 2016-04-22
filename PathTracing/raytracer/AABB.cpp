#include "AABB.h"


AABB::AABB(Point3 low,Point3 high):low(low),high(high)
{

}

AABB AABB::combine(const AABB& box1,const AABB& box2)
{
	Point3 pt1,pt2;

	pt1.x = min(box1.low.x,box2.low.x);
	pt1.y = min(box1.low.y,box2.low.y);
	pt1.z = min(box1.low.z,box2.low.z);

	pt2.x = max(box1.high.x,box2.high.x);
	pt2.y = max(box1.high.y,box2.high.y);
	pt2.z = max(box1.high.z,box2.high.z);

	return AABB(pt1,pt2);
}

bool AABB::intersect(Ray& ray,IntersectResult& resul)
{
	//²Î¿¼£ºhttp://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	float t1 = (low.x - ray.origin.x)*ray.inverseDirection.x;
	float t2 = (high.x - ray.origin.x)*ray.inverseDirection.x;
	float t3 = (low.y - ray.origin.y)*ray.inverseDirection.y;
	float t4 = (high.y - ray.origin.y)*ray.inverseDirection.y;
	float t5 = (low.z - ray.origin.z)*ray.inverseDirection.z;
	float t6 = (high.z - ray.origin.z)*ray.inverseDirection.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0) return false;
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) return false;

	return true;
}