#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../primitive/IPrimitive.h"
#include "Camera.h"
#include "../common/VMath.h"
#include "../common/IntersectResult.h"
#include "../light/ILight.h"
using std::vector;

class Scene
{
public:
	Scene();
	~Scene();
	IntersectResult intersect(Ray& ray);
	Ray* getRays(double x,double y,int pxSampleNum);
	bool isInShadow(Ray& ray,IPrimitive* light);
	Color3 phong(IntersectResult& result,Ray& ray);
	void setSize(int width,int height);
	int getWidth();
	int getHeight();

	vector<IPrimitive*> primitives;
	Camera* camera;
	Color3** color;
private:
	//´°¿Ú¿í¸ß
	int width,height;
};


#endif