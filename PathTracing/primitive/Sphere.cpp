#include "Sphere.h"

Sphere::Sphere(Point3 origin,double radius):origin(Point3(0,0,0)),radius(1.0)
{
	if(radius!=1.0) scale(radius,radius,radius);
	if(origin!=0) translate(origin.x,origin.y,origin.z);
}

IntersectResult Sphere::intersect(Ray& ray)
{
	IntersectResult result;
	Ray& transformRay = getTransformRay(ray);

	double A = Length2(transformRay.direction);
	double B = Dot(transformRay.direction,transformRay.origin);
	double C = Length2(transformRay.origin) - 1.0;
	double delta = B*B - A*C;

	if(delta>=0)
	{
		delta = sqrt(delta);
		double time = (-B - delta)/A;

		//排除光线起点在球内的情况
		if(DoubleCompare(time,0)>0)
		{
			result.point = ray.getPoint(time);
			result.distance = time;
			result.normal = getTransformNormal(transformRay.getPoint(time));
			result.primitive = this;
		}
	}
	return result;
}