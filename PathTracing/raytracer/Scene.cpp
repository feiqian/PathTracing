#include "Scene.h"
#include "../primitive/Mesh.h"

Scene::Scene()
{
	color = NULL;
	this->setSize(500,500);
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

bool Scene::intersect(Ray& ray,IntersectResult& result)
{
	//bool hit = false;
	//IntersectResult best;

	//for(int i=0,len=primitives.size();i<len;++i)
	//{
	//	if(primitives[i]->intersect(ray,best)&&best.distance<result.distance)
	//	{
	//		hit = true;
	//		result = best;
	//	}
	//}

	//return hit;
	return kdTree.intersect(ray,result);
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
			double randomX = (double)rand()/RAND_MAX;
			double randomY = (double)rand()/RAND_MAX;
			rays[i] = camera->getRay((x+randomX)*dx,(y+randomY)*dy);
		}
	}
	return rays;
}

bool Scene::isInShadow(Ray& ray,IPrimitive* light)
{
	IntersectResult shadowResult;
	if(intersect(ray,shadowResult)&&shadowResult.primitive!=light) 
		return true;
	else return false;
}

Color3 Scene::directIllumination(IntersectResult& result,Ray& ray)
{
	Color3 color;
	//TODO 没有考虑光源之间的相互遮挡
	for(int i=0,len = primitives.size();i<len;++i)
	{
		ILight* light = NULL;
		light = dynamic_cast<ILight*>(primitives[i]);
		if(light == NULL) continue;
		color += light->render(result,ray,this);
	}
	return color;
}

void Scene::focusModel()
{
	//保证OBJ文件读取的模型能全部在视锥体内（因为相机的参数被固定）
	Point3 min_xyz(DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY), 
		max_xyz(DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY);

	for(int i=0,len = primitives.size();i<len;++i)
	{
		Mesh* mesh = NULL;
		mesh = dynamic_cast<Mesh*>(primitives[i]);
		if(mesh==NULL) continue;

		const Point3& vertex = mesh->vertices[i];

		min_xyz.x = min(min_xyz.x, vertex.x);
		max_xyz.x = max(max_xyz.x, vertex.x);
		min_xyz.y = min(min_xyz.y, vertex.y);
		max_xyz.y = max(max_xyz.y, vertex.y);
		max_xyz.z = max(max_xyz.z, vertex.z);
	}

	double deltaX = -(max_xyz.x - min_xyz.x)/2-min_xyz.x,deltaY = -(max_xyz.y - min_xyz.y)/2-min_xyz.y,deltaZ = -max_xyz.z-3;
	min_xyz = Vec3(DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY), 
		max_xyz = Vec3(DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY);

	for(int i=0,len = primitives.size();i<len;++i)
	{
		Mesh* mesh = NULL;
		mesh = dynamic_cast<Mesh*>(primitives[i]);
		if(mesh==NULL) continue;

		for(int j=0,len2 = mesh->vertices.size();j<len2;++j)
		{
			Point3& vertex = mesh->vertices[j];

			//居中模型
			//vertex.x+= deltaX;
			vertex.y+= deltaY;
			//调整Z值
			vertex.z+= deltaZ;

			min_xyz.x = min(min_xyz.x, vertex.x/vertex.z);
			max_xyz.x = max(max_xyz.x, vertex.x/vertex.z);
			min_xyz.y = min(min_xyz.y, vertex.y/vertex.z);
			max_xyz.y = max(max_xyz.y, vertex.y/vertex.z);
		}
	}
	
	double rangeX = max_xyz.x - min_xyz.x,rangeY = max_xyz.y - min_xyz.y;
	rangeX = max_xyz.x - min_xyz.x,rangeY = max_xyz.y - min_xyz.y;
	double scale = max(rangeX,rangeY);

	double max_scalar;
	double cameraWidth = camera->width/camera->nearPlane ,cameraHeight = camera->height/camera->nearPlane;

	if (cameraWidth < cameraHeight) max_scalar = cameraWidth / scale;
	else max_scalar = cameraHeight / scale;

	double modelWidth = rangeX*max_scalar,modelHeight = rangeY*max_scalar;

	for(int i=0,len = primitives.size();i<len;++i)
	{
		Mesh* mesh = NULL;
		mesh = dynamic_cast<Mesh*>(primitives[i]);
		if(mesh==NULL) continue;

		mesh->resizeVertices.resize(mesh->vertices.size());

		int len2 = mesh->vertices.size();
		#pragma omp parallel
		for(int j=0;j<len2;++j)
		{
			const Point3& vertex = mesh->vertices[j];

			//缩放模型使其填满整个视口并居中
			mesh->resizeVertices[j].x = (vertex.x/vertex.z * max_scalar - modelWidth/2 -max_scalar*min_xyz.x)*vertex.z;
			mesh->resizeVertices[j].y = (vertex.y/vertex.z * max_scalar - modelHeight/2-max_scalar*min_xyz.y)*vertex.z;
			mesh->resizeVertices[j].z = vertex.z;
		}

		mesh->init();
	}
}

void Scene::init()
{
	focusModel();
	kdTree.build(primitives);
}