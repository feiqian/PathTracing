#include "Triangle.h"

IntersectResult Tri::intersect(Ray& ray)
{
	IntersectResult result;
	Ray& transformRay = getTransformRay(ray);

	double tmp = Dot(normal,transformRay.direction);

	if(DoubleCompare(tmp,0)<0)
	{
		Vec3 v = origin - transformRay.origin;

		Vec3 tmp1 = Cross(dy,transformRay.direction);
		double s = -Dot(tmp1,v)/Dot(tmp1,dx);

		Vec3 tmp2 = Cross(dx,transformRay.direction);
		double t = -Dot(tmp2,v)/Dot(tmp2,dy);

		double bestTime = Dot(normal,v)/Dot(normal,transformRay.direction);

		if(DoubleCompare(s,0)>=0&&DoubleCompare(t,0)>=0&&DoubleCompare(s+t,1)<=0&&DoubleCompare(bestTime,0)>0)
		{
			result.point = ray.getPoint(bestTime);
			result.distance = bestTime;
			result.normal = getTransformNormal(normal);
			result.primitive = this;
		}
	}
	return result;
}