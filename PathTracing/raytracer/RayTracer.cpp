#include <omp.h>
#include "RayTracer.h"
#include "../common/Parser.h"
#include "../common/GlutDisplay.h"
#include "../common/Utils.h"

const double EPS_LOOSE = 1e-8;

RayTracer::RayTracer()
{
	mcSampleNum = 1;
	pxSampleNum = 0;
	threadNum = 1;
	blockSize = 1;
	maxRecursiveDepth = 2;
}

RayTracer::~RayTracer()
{
	
}

Scene* RayTracer::getScene()
{
	return &scene;
}

Color3** RayTracer::render()
{
	int width = scene.getWidth(),height = scene.getHeight();
	double progress = 0,increment = 100.0/(width*height);

	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			//TODO do 2*2 subpixels
			int i=0;
			int cnt1=0;
			Color3 total1;
			Ray* rays = scene.getRays(x,y,pxSampleNum);

			do 
			{
				int j = 0;
				int cnt2 = 0;
				Color3 total2;

				do
				{
					Color3 rgb = Limit(trace(rays[i]),0,1);
					Color3 prev2 = j?total2 / j:Color3::NONE;

					if(Length2(rgb - prev2)<EPS_LOOSE)
					{
						++cnt2;
						if(cnt2 == 2) break;
					}
					else cnt2 = 0;

					total2+=rgb;
					++j;
				}while (j<mcSampleNum);
				total2/=j;

				Color3 prev = i?total1 / i:Color3::NONE;
				if(Length2(total1 - prev)<EPS_LOOSE)
				{
					++cnt1;
					if(cnt1 == 2) break;
				}
				else cnt1 = 0;

				total1+=total2;
				++i;
			} while (i<pxSampleNum);

			scene.color[y][x] = total1/i;
			delete [] rays;

			Utils::PrintProgress(progress,increment);
		}
	}

	return scene.color;
}
 
Color3 RayTracer::trace(Ray& ray,int currDepth,Vec3 weight)
{
	if(currDepth>=maxRecursiveDepth || Length2(weight)<EPS_LOOSE) 
		return Color3::BLACK;

	IntersectResult& result = scene.intersect(ray);
	if(!result.isHit())  return Color3::BLACK;
	else 
	{
		Material& attr = result.primitive->attr;
		Vec3 reflection = attr.kd+attr.ks+attr.kt;

		Color3 indirectIllumination;
		Ray& newRay = mcSelect(ray,result);

		if(newRay.souce != SOURCE::NONE)
		{
			indirectIllumination = trace(newRay,currDepth+1,reflection*weight);

			switch(newRay.souce)
			{
			case SOURCE::DIFFUSE_REFLECT:
				indirectIllumination = attr.kd*indirectIllumination;
				break;
			case SOURCE::SPECULA_REFLECT:
				indirectIllumination = attr.ks*indirectIllumination*Dot(newRay.direction,result.normal);
				break;
			case SOURCE::TRANSMISSON:
				//TODO
				break;
			}
		}

		Color3& directIllumination= scene.directIllumination(result,ray);

		return attr.emission + directIllumination + indirectIllumination;
	}
}

Ray RayTracer::mcSelect(Ray& ray,IntersectResult& result)
{
	Ray newRay;
	newRay.souce = SOURCE::NONE;
	Material& attr = result.primitive->attr;

	double num[3];
	num[0]= Dot(attr.kd,Vec3(1,1,1));
	num[1]= Dot(attr.ks,Vec3(1,1,1))+ num[0];
	num[2]= Dot(attr.kt,Vec3(1,1,1))+ num[1];

	if(num[2]<=0) return newRay;

	double randNum = (double)rand()/RAND_MAX * num[2];
	if(randNum<num[0]) 
	{
		newRay.direction = importanceSampleUpperHemisphere(result.normal);
		newRay.souce = SOURCE::DIFFUSE_REFLECT;
	}
	else if(randNum<num[1])
	{
		Vec3 prefectReflectDirection = Reflect(ray.direction,result.normal);
		newRay.direction = importanceSampleUpperHemisphere(prefectReflectDirection,result.primitive->attr.shiness);
		newRay.souce = SOURCE::SPECULA_REFLECT;
	}
	else
	{
		double cosAngle = Dot(ray.direction,result.normal);
		double n;
		newRay.souce = SOURCE::TRANSMISSON;

		if(cosAngle>0) 
		{
			n = result.primitive->attr.refractiveIndex;
			if(!Refract(ray.direction,-result.normal,n,newRay.direction)) newRay.souce = SOURCE::NONE;
		}
		else
		{
			n = 1.0/result.primitive->attr.refractiveIndex;
			if(!Refract(ray.direction,result.normal,n,newRay.direction)) newRay.souce = SOURCE::NONE;
		}
	
	}

	newRay.origin = result.point;
	return newRay;
}

Vec3 RayTracer::importanceSampleUpperHemisphere(Vec3& up, double n)
{
	double phi, theta;
	double r1 = (double)rand() / RAND_MAX , r2 = (double)rand() / RAND_MAX;

	phi = r1 * 2 * PI;
	theta = n<0?asin(sqrt(r2)) : acos(pow(r2, 1/(n+1)));
	Vec3 sample(sin(theta)*cos(phi),cos(theta),sin(theta)*sin(phi));

	Vec3 front,right;

	if (fabs(up.x) > fabs(up.y)) 
		front = Vec3(up.z, 0, -up.x); 
	else 
		front = Vec3(0, -up.z, up.y); 

	front = Normalize(front);
	right = Cross(up,front);

	Mat4 mat4(right,up,front);
	return Normalize(mat4*sample);
}

void RayTracer::run(string obj_file)
{
	if(Parser::parse(obj_file,&scene))
	{
		scene.focusModel();
		GlutDisplay::setRayTracer(this);
		GlutDisplay::render();
	}
}
