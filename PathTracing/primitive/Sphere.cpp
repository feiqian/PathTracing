#include "Sphere.h"

Sphere::Sphere(Point3 origin,double radius):origin(origin),radius(radius)
{

}

bool Sphere::intersect(Ray& ray,IntersectResult& result)
{
	double A = Length2(ray.direction);
	double B = Dot(ray.direction,ray.origin-origin);
	double C = Length2(ray.origin-origin) - radius*radius;
	double delta = B*B - A*C;

	if(delta>=0)
	{
		delta = sqrt(delta);
		double time1 = (-B - delta)/A;
		double time2 = (-B + delta)/A;

		time1 = ray.isWithinBounds(time1)?time1:-1;
		time2 = ray.isWithinBounds(time2)?time2:-1;

		if(time1>0&&time2>0)
		{
			result.point = ray.getPoint(time1);
			result.distance = time1;
			result.normal = Normalize(result.point-origin);
			result.primitive = this;
			ray.tMax = time1;
			return true;
		}
		else if(time1<0&&time2>0)
		{
			result.point = ray.getPoint(time2);
			result.distance = time2;
			result.normal = -Normalize(result.point-origin);
			result.primitive = this;
			ray.tMax = time2;
			return true;
		}
	}
	return false;
}

AABB Sphere::getAABB()
{
	return AABB(origin-radius,origin+radius);
}

Point2 Sphere::getTextureCoordinate(const Point3& point)
{
	double theta = acos((point.y - origin.y) / radius);
	double phi = atan2(point.z - origin.y, point.x - origin.x);
	phi = (phi < 0 ? phi + 2*M_PI : phi);
	double u = 1 - (phi / (2 * M_PI));
	double v = (M_PI - theta) / M_PI;
	return Point2(u,v);
}