#include "AmbientLight.h"

AmbientLight::AmbientLight(Color3 color):color(color)
{

}

Color3 AmbientLight::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	Material& intersectAttr = result.primitive->attr;
	Reflectance& ref = result.primitive->getReflectance(result.point);
	return ref.ka*color;
}