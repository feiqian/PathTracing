#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__
#include <thread>
#include <mutex>
#include "Scene.h"
#include "Ray.h"

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	Color3** render();
	void setScene(Scene* scene);
	Scene* getScene();
private:
	void thread_task();
	Color3 trace(Ray& ray,int currDepth = 0,Vec3 weight = Vec3(1,1,1));
	Ray mcSelect(Ray& ray,IntersectResult& result);
	Vec3 importanceSampleUpperHemisphere(Vec3& upVector, double n=-1);

	//TODO blockSize
	int blockSize;//���ؿ��С
	int pxSampleNum;//ÿ�����صĲ�����Ŀ
	int mcSampleNum;//���ؿ��������Ŀ
	int threadNum;//��Ⱦ�߳���Ŀ
	int maxRecursiveDepth;//�������ݹ����

	Scene* scene;
	std::thread *threadPool;
	std::mutex mtx;
	int nx,ny;//��һ��Ҫ������Ⱦ����Ļ����
};

#endif