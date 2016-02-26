#ifndef __MATERIAL_ATTRIBUTE_H__
#define __MATERIAL_ATTRIBUTE_H__
#include "VMath.h"

struct MaterialAttribute
{
	MaterialAttribute():emission(Color3::NONE){};
	Color3 color;
	Color3 emission;
	double roughness;

	Vec3 ka;//环境光系数
	Vec3 kd;//漫反射系数
	Vec3 ks;//镜面反射系数
	Vec3 kt;//透射系数
};

#endif