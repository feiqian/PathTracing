#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "VMath.h"

//http://paulbourke.net/dataformats/mtl/

struct Material
{
	Material():shiness(100),refractiveIndex(1.0){};

	Color3 color;
	Color3 emission;

	double shiness;//�����Ns
	double refractiveIndex;//����ϵ��Ni

	Vec3 ka;//������ϵ��
	Vec3 kd;//������ϵ��
	Vec3 ks;//���淴��ϵ��
	Vec3 kt;//͸��ϵ��
	Vec3 tf;//transmission filter
};

#endif