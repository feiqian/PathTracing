#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__
#include "../primitive/Sphere.h"
#include "ILight.h"

class PointLight : public Sphere,public ILight
{
public:
	PointLight(Point3 origin = Vec3(0,0,0),Color3 emission = Color3::WHITE,float intense=1.0):Sphere(origin)
	{
		attr.emission = emission;
		this->intense = intense;
		this->origin = origin;
	};
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);
private:
	double intense;
};

#endif