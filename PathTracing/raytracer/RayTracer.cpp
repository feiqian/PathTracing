#include <omp.h>
#include "RayTracer.h"
#include "../common/Parser.h"
#include "../common/GlutDisplay.h"
#include "../common/Utils.h"
#include "../common/Timer.h"

#define NUM_THREADS 20
const double EPS_LOOSE = 1e-8;

double getFresnelIndex(double ni,double nt,double cosTheta)
{
	//fresnel反射系数的schlick近似
	double f0 = (ni-nt)/(ni+nt); 
	f0*=f0;
	double schlick = f0 + (1-f0)* pow(1.0-cosTheta,5);

	return schlick;
}

RayTracer::RayTracer()
{
	mcSampleNum = 20;
	pxSampleNum = 0;
	blockSize = 1;
	maxRecursiveDepth = 5;
	useDirectLight = true;
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
	TotalTimer renderTimer("rendering");
	int width = scene.getWidth(),height = scene.getHeight();
	double progress = 0,increment = 100.0/(width*height);

	//只在Release模式下开启了OMP支持
	#pragma omp parallel for num_threads(NUM_THREADS)
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

			#ifdef _DEBUG
				Utils::PrintProgress(progress,increment);
			#endif // DEBUG
		}
	}

	return scene.color;
}
 
Color3 RayTracer::trace(Ray& ray,int currDepth,Vec3 weight)
{
	IntersectResult result;	
	if(!scene.intersect(ray,result))  return Color3::BLACK;
	else 
	{
		Material& attr = result.primitive->attr;
		Vec3 reflection = attr.kd+attr.ks;

		if(currDepth>=maxRecursiveDepth) 
			return attr.emission;

		//如果当前是间接光并且使用了直接光照，一旦hit了一个光源时，我们应该返回空，这样可以避免对直接光照的过采样
		if(currDepth&&useDirectLight&&attr.emission!=Color3::BLACK) return Color3::BLACK;

		Color3 indirectIllumination,directIllumination;
		double survivalContribution;
		Ray& newRay = mcSelect(ray,result,survivalContribution);

		if(newRay.source != SOURCE::NONE)
		{
			indirectIllumination = trace(newRay,currDepth+1);

			switch(newRay.source)
			{
			case SOURCE::DIFFUSE_REFLECT:
				indirectIllumination = attr.kd*survivalContribution*indirectIllumination;
				break;
			case SOURCE::SPECULAR_REFLECT:
				indirectIllumination = attr.ks*survivalContribution*indirectIllumination*Dot(newRay.direction,result.normal);
				break;
			case SOURCE::TRANSMISSON:
				indirectIllumination = attr.ks*survivalContribution*indirectIllumination;
				break;
			}
		}

		if(useDirectLight) directIllumination = scene.directIllumination(result,ray);

		return attr.emission + directIllumination + indirectIllumination;
	}
}

Ray RayTracer::mcSelect(Ray& ray,IntersectResult& result,double& survivalContribution)
{
	Material& attr = result.primitive->attr;
	survivalContribution = 1.0;

	Vec3 direction;
	double num[2];
	num[0]= Dot(attr.kd,Vec3(1,1,1));
	num[1]= Dot(attr.ks,Vec3(1,1,1))+ num[0];

	if(num[1]<=0) return Ray(result.point,direction);

	if(attr.bUseFresnel&&attr.refractiveIndex!=1.0)
	{
		double ni,nt;
		double bInto = Dot(ray.direction,result.normal)>0?false:true;

		if(!bInto) 
		{
			ni = attr.refractiveIndex;
			nt = 1.0;
		}
		else
		{
			ni= 1.0;
			nt = attr.refractiveIndex;
		}

		if(Refract(ray.direction,bInto?result.normal:-result.normal,ni/nt,direction)) 
		{
			double cosTheta = bInto?Dot(-ray.direction,result.normal):Dot(direction,result.normal);
			double fresnelIndex = getFresnelIndex(ni,nt,cosTheta);

			//Reflection or refraction?
			double transmissonSurvival;
			if(russianRoulette(fresnelIndex,transmissonSurvival))
			{
				survivalContribution *= 1.0/transmissonSurvival * (1.0-fresnelIndex);
				return Ray(result.point,direction,SOURCE::TRANSMISSON);
			}
			else survivalContribution *= 1.0/(1.0-transmissonSurvival) * fresnelIndex;
		}
	}
	
	//If reflection : diffuse or specular?
	double specularSurvival;
	if(russianRoulette(num[0]/num[1],specularSurvival))
	{
		survivalContribution *= 1.0/specularSurvival;
		Vec3& prefectReflectDirection = Reflect(ray.direction,result.normal);
		direction = importanceSampleUpperHemisphere(prefectReflectDirection,attr.shiness);
		return Ray(result.point,direction,SOURCE::SPECULAR_REFLECT);
	}
	else
	{
		survivalContribution *= 1.0/(1.0-specularSurvival);
		direction = importanceSampleUpperHemisphere(result.normal);
		return Ray(result.point,direction,SOURCE::DIFFUSE_REFLECT);
	}
}

bool RayTracer::russianRoulette(double probability, double& survival)
{
	survival = (double)rand()/RAND_MAX;
	if (survival > probability) return true;
	return false;
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
		scene.init();
		GlutDisplay::setRayTracer(this);
		GlutDisplay::render();
	}
}
