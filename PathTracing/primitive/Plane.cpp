#include "Plane.h"

Plane::Plane(Point3 origin,Vec3 dx,Vec3 dy)
	:origin(origin),dx(dx),dy(dy)
{
	normal = Normalize(Cross(dx,dy));
}

IntersectResult Plane::intersect(Ray& ray)
{
	IntersectResult result;

	double tmp = Dot(normal,ray.direction);

	if(DoubleCompare(tmp,0)<0)
	{
		Vec3 v = origin - ray.origin;

		Vec3 tmp1 = Cross(dy,ray.direction);
		double s = -Dot(tmp1,v)/Dot(tmp1,dx);
		
		Vec3 tmp2 = Cross(dx,ray.direction);
		double t = -Dot(tmp2,v)/Dot(tmp2,dy);

		double bestTime = Dot(normal,v)/Dot(normal,ray.direction);

		if(DoubleCompare(s,0)>=0&&DoubleCompare(s,1)<=0&&DoubleCompare(t,0)>=0&&DoubleCompare(t,1)<=0&&DoubleCompare(bestTime,0)>0)
		{
			result.point = ray.getPoint(bestTime);
			result.distance = bestTime;
			result.normal = normal;
			result.primitive = this;
		}
	}
	return result;
}