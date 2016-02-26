#ifndef __PARSER_H__
#define __PARSER_H__
#include "../raytracer/Scene.h"
#include "../primitive/IPrimitive.h"
#include "../primitive/Sphere.h"
#include "../primitive/Cube.h"
#include "../primitive/Plane.h"
#include "../light/PointLight.h"

struct Parser
{
	static void parse(const char* fileName,Scene* scene);
};


#endif