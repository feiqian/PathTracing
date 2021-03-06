#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../primitive/IPrimitive.h"
#include "Camera.h"
#include "../common/VMath.h"
#include "../common/IntersectResult.h"
#include "../light/ILight.h"
#include "KdTree.h"
using std::vector;

struct ILight;
class Scene
{
public:
	Scene();
	~Scene();
	
	void init();
	bool intersect(Ray& ray,IntersectResult& result);
	Ray* getRays(double x,double y,int pxSampleNum);
	bool isInShadow(Ray& ray);
	Color3 directIllumination(IntersectResult& result,Ray& ray);
	void setSize(int width,int height);

	inline int getWidth(){return width;}
	inline int getHeight(){return height;}

	vector<IPrimitive*> primitives;
	vector<ILight*> lights;
	Camera* camera;
	float* color;
private:
	void focusModel();

	KdTree kdTree;
	//���ڿ���
	int width,height;
};


#endif