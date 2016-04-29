#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__
#include <string>
#include "Scene.h"
#include "Ray.h"
using namespace std;

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	void run(string obj_file);
	Scene* getScene();
	float* render();
private:
	Color3 trace(Ray& ray,int currDepth = 0,Vec3 weight = Vec3(1,1,1));
	Ray mcSelect(Ray& ray,IntersectResult& result,double& survival);
	bool russianRoulette(double probability, double& survivor);
	Vec3 importanceSampleUpperHemisphere(Vec3& upVector, double n=-1);
	void writeResultImage();

	//TODO blockSize
	int blockSize;//像素块大小
	int pxSampleNum;//每个像素的采样数目
	int mcSampleNum;//蒙特卡洛采样数目
	int maxRecursiveDepth;//光线最大递归深度
	bool useDirectLight;//是否使用直接光照
	string resultPath;

	int iterations;
	Scene scene;
};

#endif