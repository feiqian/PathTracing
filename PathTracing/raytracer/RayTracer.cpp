#include "RayTracer.h"
const double EPS_LOOSE = 1e-6;

RayTracer::RayTracer()
{
	mcSampleNum = 200;
	pxSampleNum = 1;
	threadNum = 10;
	blockSize = 1;
	maxRecursiveDepth = 100;

	scene = NULL;
	threadPool = new std::thread[threadNum];
}

RayTracer::~RayTracer()
{
	delete [] threadPool;
}

Scene* RayTracer::getScene()
{
	return scene;
}

void RayTracer::setScene(Scene* scene)
{
	this->scene = scene;
}

Color3** RayTracer::render()
{
	nx=0;
	ny=0;

	for(int i=0;i<threadNum;++i)
	{
		threadPool[i] = std::thread(std::bind(&RayTracer::thread_task, this));
	}
	for(int i=0;i<threadNum;++i)
	{
		threadPool[i].join();
	}

	return scene->color;
}

void RayTracer::thread_task()
{
	while (true)
	{
		mtx.lock();
		int x = nx;
		int y = ny;

		++nx;
		if(nx == scene->getWidth()){
			nx = 0;
			++ny;
		}
		mtx.unlock();

		if(y >= scene->getHeight()) 
			break;
		printf("%d %d\n",x,y);

		//TODO do 2*2 subpixels
		int i=0;
		int cnt1=0;
		Color3 total1;

		do 
		{
			int j = 0;
			int cnt2 = 0;
			Color3 total2;
			Ray* rays = scene->getRays(x,y,pxSampleNum>1?1:0);

			do
			{
				Color3 rgb = Limit(trace(rays[0]),0,1);
				if(rgb!=Color3::BLACK) ++cnt2;
				Color3 prev2 = j?total2 / j:Color3::BLACK;

				/*	if(Length2(rgb - prev2)<EPS_LOOSE)
				{
				++cnt2;
				if(cnt2 == 2) break;
				}
				else cnt2 = 0;*/

				total2+=rgb;
				++j;
			}while (j<mcSampleNum);
			if(cnt2) total2/=cnt2;

			Color3 prev = i?total1 / i:Color3::BLACK;
			if(Length2(total1 - prev)<EPS_LOOSE)
			{
				++cnt1;
				if(cnt1 == 2) break;
			}
			else cnt1 = 0;

			total1+=total2;
			++i;
			delete [] rays;
		} while (i<pxSampleNum);

		scene->color[y][x] = total1/i;
	}
}

Color3 RayTracer::trace(Ray& ray,int currDepth,Vec3 energy)
{
	if(currDepth>=maxRecursiveDepth) return Color3::BLACK;

	//TODO没有判断可能直接和光源相交
	IntersectResult& result = scene->intersect(ray);
	if(!result.isHit()) return scene->bgColor;
	else 
	{
		MaterialAttribute& attr = result.primitive->attr;
		if(attr.emission!=Color3::NONE) return attr.emission;
		else
		{
			Color3 color;
			Ray& newRay = mcSelect(ray,result);
			Color3 color2 = trace(newRay,++currDepth);

			if(newRay.souce == SOURCE::DIFFUSE_REFLECT) color+=color2.multiple(attr.kd);
			else if(newRay.souce == SOURCE::SPECULA_REFLECT) color+=color2.multiple(attr.ks);
			else color+=color2.multiple(attr.kt);

			return color;
		}
	}
}

Ray RayTracer::mcSelect(Ray& ray,IntersectResult& result)
{
	Ray newRay;
	MaterialAttribute& attr = result.primitive->attr;
	double num[3];
	num[0]= Dot(attr.kd,Vec3(1,1,1));
	num[1]= Dot(attr.ks,Vec3(1,1,1))+ num[0];
	num[2]= Dot(attr.kt,Vec3(1,1,1))+ num[1];

	double randNum = (double)rand()/RAND_MAX * num[2];
	if(randNum<num[0]) 
	{
		//非次级表面散射
		do 
		{
			newRay.direction.x = (double)rand() / RAND_MAX * 2 - 1;
			newRay.direction.y = (double)rand() / RAND_MAX * 2 - 1;
			newRay.direction.z = (double)rand() / RAND_MAX * 2 - 1;
		} while (Length2(newRay.direction)<EPS || Dot(newRay.direction,result.normal)<0);
		newRay.direction = Normalize(newRay.direction);
		newRay.souce = SOURCE::DIFFUSE_REFLECT;
	}
	else if(randNum<num[1])
	{
		newRay.direction = Reflect(ray.direction,result.normal);
		newRay.souce = SOURCE::SPECULA_REFLECT;
	}
	else
	{
		newRay.direction = Refract(ray.direction,result.normal,1);
		newRay.souce = SOURCE::TRANSMISSON;
	}

	newRay.origin = result.point - result.normal*EPS;
	return newRay;
}
