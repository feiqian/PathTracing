#ifndef __MATERIAL_ATTRIBUTE_H__
#define __MATERIAL_ATTRIBUTE_H__
#include "VMath.h"

struct MaterialAttribute
{
	MaterialAttribute():emission(Color3::NONE){};
	Color3 color;
	Color3 emission;
	double roughness;

	Vec3 ka;//������ϵ��
	Vec3 kd;//������ϵ��
	Vec3 ks;//���淴��ϵ��
	Vec3 kt;//͸��ϵ��
};

#endif