#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include <string>
#include "VMath.h"
#include "FreeImage/FreeImage.h"
using namespace std;

//http://paulbourke.net/dataformats/mtl/

class Texture {
public:
	Texture(string filename);
	Color3 getColor(const Vec2& uvCoord);

private:
	FIBITMAP* image;
	unsigned int height;
	unsigned int width;
};

//The material parameters that can be modified by a texture map
struct Reflectance {
	Vec3 ka;//������ϵ��
	Vec3 kd;//������ϵ��
	Vec3 ks;//���淴��ϵ��
};

struct Material
{
	Material():shiness(60),refractiveIndex(1.0),kaTexture(NULL),kdTexture(NULL),ksTexture(NULL),bUseFresnel(false){};

	std::string name;
	Color3 emission;

	double shiness;//�����Ns
	double refractiveIndex;//����ϵ��Ni
	Vec3 tf;//transmission filter

	Texture* kaTexture;
	Texture* kdTexture;
	Texture* ksTexture;

	bool bUseFresnel;
	Reflectance ref;
};

#endif