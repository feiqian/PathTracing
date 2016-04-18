#ifndef __ILIGHT_H__
#define __ILIGHT_H__
#include "../common/IntersectResult.h"
#include "../raytracer/Ray.h"
#include "../common./Material.h"
#include "../raytracer/Scene.h"

class Scene;
struct ILight
{
	virtual Color3 render(IntersectResult& result,Ray& ray,Scene* scene) = 0;
};

#endif