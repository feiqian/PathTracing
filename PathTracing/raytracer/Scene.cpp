#include "Scene.h"

Scene::Scene()
{
	color = NULL;
	this->setSize(500,500);
	bgColor = Vec3::BLACK;
}

Scene::~Scene()
{
	if(color!=NULL)
	{
		for(int i=0;i<height;++i) delete [] color[i];
		delete [] color;
		color = NULL;
	}
}

void Scene::setSize(int width,int height)
{
	if(!width||!height) return;
	if(width>this->width||height>this->height)
	{
		if(color!=NULL)
		{
			for(int i=0;i<height;++i) delete [] color[i];
			delete [] color;
			color = NULL;
		}

		color = new Color3*[height];
		for(int i=0,len=height;i<len;++i) color[i] = new Color3[width];
	}
	this->width = width;
	this->height = height;
}

int Scene::getWidth(){return width;}
int Scene::getHeight(){return height;}

IntersectResult Scene::intersect(Ray& ray)
{
	IntersectResult best;
	best.distance = 1e20;

	for(std::vector<IPrimitive*>::iterator it = primitives.begin();it!=primitives.end();++it)
	{
		IntersectResult& result = (*it)->intersect(ray);
		if(result.isHit()&&result.distance<best.distance) best = result;
	}
	return best;
}

Ray* Scene::getRays(double x,double y,int pxSampleNum)
{
	double dx=1.0f/width,dy=1.0f/height;
	Ray* rays = new Ray[pxSampleNum?pxSampleNum:1];
	if(pxSampleNum == 0) rays[0]= camera->getRay(x*dx,y*dy);
	else
	{
		for(int i=0;i<pxSampleNum;++i)
		{
			double randomX = (double)rand()/RAND_MAX*dx;
			double randomY = (double)rand()/RAND_MAX*dy;
			rays[i] = camera->getRay(x+randomX,y+randomY);
		}
	}
	return rays;
}

bool Scene::isInShadow(Ray& ray)
{
	for(std::vector<IPrimitive*>::iterator it = primitives.begin();it!=primitives.end();++it)
	{
		IntersectResult& result = (*it)->intersect(ray);
		if(result.isHit()) return true;
	}
	return false;
}