#include "ILight.h"

class AreaLight : public ILight
{
public:
	AreaLight(Point3 origin,Vec3 dx,Vec3 dy,Color3 emission);

	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);

	int lightSamples;
private:
	Point3 origin;
	Vec3 dx,dy;
	Vec3 normal;
	Vec3 emission;
	double area;
};