#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__
#include "../primitive/Sphere.h"
#include "ILight.h"

class PointLight : public Sphere,public ILight
{
public:
	PointLight(Point3 origin = Vec3(0,0,0),Color3 emission = Color3::WHITE):Sphere(origin)
	{
		attr.emission = emission;
		this->origin = origin;
	};
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);
};

#endif