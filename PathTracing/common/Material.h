#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "VMath.h"

//http://paulbourke.net/dataformats/mtl/

struct Material
{
	Material():shiness(100),refractiveIndex(1.0){};

	Color3 color;
	Color3 emission;

	double shiness;//光泽度Ns
	double refractiveIndex;//折射系数Ni

	Vec3 ka;//环境光系数
	Vec3 kd;//漫反射系数
	Vec3 ks;//镜面反射系数
	Vec3 kt;//透射系数
	Vec3 tf;//transmission filter
};

#endif