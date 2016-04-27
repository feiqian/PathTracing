#include "Scene.h"
#include "../primitive/Mesh.h"
#include "../light/PointLight.h"

Scene::Scene()
{
	color = NULL;
	width = 0;
	height = 0;
	this->setSize(500,500);
}

Scene::~Scene()
{
	if(color!=NULL)
	{
		delete [] color;
		color = NULL;
	}
}

void Scene::setSize(int width,int height)
{
	if(!width||!height) return;
	if(width==this->width&&height==this->height) return;

	if(color!=NULL) delete [] color;
	color = new float[width*height*3];

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
	for(int i=0,len=lights.size();i<len;++i)
	{
		color += lights[i]->render(result,ray,this);
	}
	return color;
}

void Scene::focusModel()
{
	Point3 min_xyz,max_xyz;
	//保证OBJ文件读取的模型能全部在视锥体内（因为相机的参数被固定）
	min_xyz = Vec3(DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY), 
	max_xyz = Vec3(DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY);

	for(int i=0,len = primitives.size();i<len;++i)
	{
		Mesh* mesh = NULL;
		mesh = dynamic_cast<Mesh*>(primitives[i]);
		if(mesh==NULL) continue;

		for(int j=0,len2 = mesh->vertices.size();j<len2;++j)
		{
			const Point3& vertex = mesh->vertices[j];

			min_xyz.x = min(min_xyz.x, vertex.x);
			max_xyz.x = max(max_xyz.x, vertex.x);
			min_xyz.y = min(min_xyz.y, vertex.y);
			max_xyz.y = max(max_xyz.y, vertex.y);
			max_xyz.z = max(max_xyz.z, vertex.z);
		}
	}

	double deltaX = -(max_xyz.x - min_xyz.x)/2-min_xyz.x,deltaY = -(max_xyz.y - min_xyz.y)/2-min_xyz.y,deltaZ = -(max_xyz.z+1);
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
			vertex.x+= deltaX;
			vertex.y+= deltaY;
			//初步调整Z值
			vertex.z+= deltaZ;

			min_xyz.x = min(min_xyz.x, vertex.x/vertex.z);
			max_xyz.x = max(max_xyz.x, vertex.x/vertex.z);
			min_xyz.y = min(min_xyz.y, vertex.y/vertex.z);
			max_xyz.y = max(max_xyz.y, vertex.y/vertex.z);
		}
	}

	double cameraWidth = camera->width/camera->nearPlane ,cameraHeight = camera->height/camera->nearPlane;
	double rangeX = max_xyz.x - min_xyz.x,rangeY = max_xyz.y - min_xyz.y;
	deltaZ = -max(rangeX,rangeY)/min(cameraWidth,cameraHeight)*1.0;

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

			//继续调整Z值
			vertex.z+= deltaZ;

			min_xyz.x = min(min_xyz.x, vertex.x/vertex.z);
			max_xyz.x = max(max_xyz.x, vertex.x/vertex.z);
			min_xyz.y = min(min_xyz.y, vertex.y/vertex.z);
			max_xyz.y = max(max_xyz.y, vertex.y/vertex.z);
		}
	}
	
	rangeX = max_xyz.x - min_xyz.x,rangeY = max_xyz.y - min_xyz.y;
	double scale = min(cameraWidth,cameraHeight)/max(rangeX,rangeY);
	double modelWidth = rangeX*scale,modelHeight = rangeY*scale;
	double minX = min_xyz.x,minY = min_xyz.y;

	min_xyz = Vec3(DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY,DOUBLE_POSITIVE_INFINITY), 
	max_xyz = Vec3(DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY,DOUBLE_NEGATIVE_INFINITY);

	for(int i=0,len = primitives.size();i<len;++i)
	{
		Mesh* mesh = NULL;
		mesh = dynamic_cast<Mesh*>(primitives[i]);
		if(mesh==NULL) continue;

		mesh->resizeVertices.resize(mesh->vertices.size());

		int len2 = mesh->vertices.size();
		#pragma omp parallel for num_threads(20)
		for(int j=0;j<len2;++j)
		{
			const Point3& vertex = mesh->vertices[j];
			Point3& resizeVertex = mesh->resizeVertices[j];

			//缩放模型使其填满整个视口并居中
			resizeVertex.x = (vertex.x/vertex.z * scale - modelWidth/2 -scale*minX)*vertex.z;
			resizeVertex.y = (vertex.y/vertex.z * scale - modelHeight/2-scale*minY)*vertex.z;
			resizeVertex.z = vertex.z;
		}

		mesh->init();
	}
}

void Scene::init()
{
	focusModel();

	for(int i=0,len = primitives.size();i<len;++i)
	{
		ILight* light = NULL;
		light = dynamic_cast<ILight*>(primitives[i]);
		if(light == NULL) continue;
		lights.push_back(light);
	}

	kdTree.build(primitives);
}