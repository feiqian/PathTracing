#include "Cube.h"

Cube::Cube(Point3 origin/* =Vec3(0,0,0) */,double length/* =1.0 */,double width/* =1.0 */,double height/* =1.0 */)
	:origin(Point3(0,0,0)),length(1.0),width(1.0),height(1.0)
{
	if(length!=1.0||width!=1.0||height!=1.0) scale(length,height,width);
	if(origin!=0) translate(origin.x,origin.y,origin.z);
}

bool Cube::intersect(Ray& ray,IntersectResult& result)
{
	Ray& transformRay = getTransformRay(ray);

	double tIn = -1000000,tOut = 1000000;
	double number,demon;
	double tHit;
	Vec3 normal,inNormal;

	for(int i=0;i<6;++i)
	{
		switch (i)
		{
		case 0:
			number = 0.5 - transformRay.origin.y;
			demon = transformRay.direction.y;
			normal = Vec3(0,1,0);
			break;
		case 1:
			number = 0.5 + transformRay.origin.y;
			demon = -transformRay.direction.y;
			normal = Vec3(0,-1,0);
			break;
		case 2:
			number = 0.5 - transformRay.origin.x;
			demon = transformRay.direction.x;
			normal = Vec3(1,0,0);
			break;
		case 3:
			number = 0.5 + transformRay.origin.x;
			demon = -transformRay.direction.x;
			normal = Vec3(-1,0,0);
			break;
		case 4:
			number = 0.5 - transformRay.origin.z;
			demon = transformRay.direction.z;
			normal = Vec3(0,0,1);
			break;
		case 5:
			number = 0.5 + transformRay.origin.z;
			demon = -transformRay.direction.z;
			normal = Vec3(0,0,-1);
			break;
		default:
			break;
		}

		//光线与平面平行
		if(DoubleEquals(demon,0.0)) 
		{
			if(number<0) return false;
		}
		else
		{
			tHit = number/demon;
			if(demon>0) 
			{
				if(tHit<tOut)
				{
					tOut = tHit;
				}
			}
			else 
			{
				if(tHit>tIn)
				{
					tIn = tHit;
					inNormal = normal;
				}
			}
			if(DoubleCompare(tIn,tOut)>0) return false;
		}
	}
	
	if(ray.isWithinBounds(tIn))
	{
		result.point = ray.getPoint(tIn);
		result.normal = getTransformNormal(inNormal);
		result.distance = tIn;
		result.primitive = this;
		return true;
	}
	return false;
}

AABB Cube::getAABB()
{
	return AABB(origin,origin+Vec3(length,height,width));
}