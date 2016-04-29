#include <omp.h>
#include "windows.h"
#include "RayTracer.h"
#include "../common/Parser.h"
#include "../common/GlutDisplay.h"
#include "../common/Utils.h"
#include "../common/Timer.h"
#include "FreeImage/FreeImage.h"

#define NUM_THREADS 10
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
	mcSampleNum = 100;
	pxSampleNum = 1;
	blockSize = 1;
	maxRecursiveDepth = 5;
	useDirectLight = true;
	iterations = 0;
}

RayTracer::~RayTracer()
{
	
}

Scene* RayTracer::getScene()
{
	return &scene;
}

float* RayTracer::render()
{
	++iterations;

	if(iterations>mcSampleNum)
	{
		if(iterations-1==mcSampleNum) writeResultImage();
		return scene.color;
	}

	static CONSOLE_SCREEN_BUFFER_INFO info;
	if(!iterations) GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
	else SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),info.dwCursorPosition);
	
	TotalTimer renderTimer("current iteration : "+to_string(iterations));

	int width = scene.getWidth(),height = scene.getHeight();
	//只在Release模式下开启了OMP支持
	#pragma omp parallel for schedule(dynamic, 1) num_threads(NUM_THREADS)
	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			Color3 color;
			Ray* rays = scene.getRays(x,y,pxSampleNum);

			for(int k=0;k<pxSampleNum;++k)
			{
				Color3& result = Limit(trace(rays[k]),0,1);
				color+=result;
			}

			color = color/pxSampleNum;
			delete [] rays;

			int index =  y*width*3+x*3;
			scene.color[index] = (scene.color[index] * (iterations-1) + color.r)/iterations;
			scene.color[index+1] = (scene.color[index+1] * (iterations-1) + color.g)/iterations;
			scene.color[index+2] = (scene.color[index+2] * (iterations-1) + color.b)/iterations;
		}
	}

	return scene.color;
}

Color3 RayTracer::trace(Ray& ray,int currDepth,Vec3 weight)
{
	IntersectResult result;	
	if(!scene.intersect(ray,result)) return Color3::BLACK;
	else 
	{
		Material& attr = result.primitive->attr;

		if(currDepth>=maxRecursiveDepth) 
			return attr.emission;

		//如果当前是间接光并且使用了直接光照，一旦hit了一个光源时，我们应该返回空，这样可以避免对直接光照的过采样
		if(currDepth&&useDirectLight&&attr.emission!=Color3::BLACK) return Color3::BLACK;

		Color3 indirectIllumination,directIllumination;
		double survivalContribution;

		Reflectance& ref = result.primitive->getReflectance(result.point);
		Ray& newRay = mcSelect(ray,result,survivalContribution);

		if(newRay.source != SOURCE::NONE)
		{
			indirectIllumination = trace(newRay,currDepth+1);

			switch(newRay.source)
			{
			case SOURCE::DIFFUSE_REFLECT:
				indirectIllumination = ref.kd*survivalContribution*indirectIllumination;
				break;
			case SOURCE::SPECULAR_REFLECT:
				indirectIllumination = ref.ks*survivalContribution*indirectIllumination*Dot(newRay.direction,result.normal);
				break;
			case SOURCE::TRANSMISSON:
				indirectIllumination = survivalContribution*indirectIllumination;
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
	Reflectance& ref = result.primitive->getReflectance(result.point);
	survivalContribution = 1.0;

	Vec3 direction;
	double num[2];
	num[0]= Dot(ref.kd,Vec3(1,1,1));
	num[1]= Dot(ref.ks,Vec3(1,1,1))+ num[0];

	if(num[1]<=0) return Ray(result.point,direction);

	if(attr.bUseFresnel&&attr.refractiveIndex!=1.0)
	{
		double ni,nt;
		double cosTheta = Dot(ray.direction,result.normal);
		Vec3 normal = cosTheta<=0.0?result.normal:-result.normal;

		if(cosTheta>0.0) 
		{
			ni = attr.refractiveIndex;
			nt = 1.0;
		}
		else
		{
			ni= 1.0;
			nt = attr.refractiveIndex;
		}

		//Reflection or refraction?
		double fresnelIndex = getFresnelIndex(ni,nt,abs(cosTheta));
		double transmissonSurvival;
		if(russianRoulette(fresnelIndex,transmissonSurvival))
		{
			//survivalContribution *= 1.0/transmissonSurvival;
			if(Refract(ray.direction,normal,ni/nt,direction)) 
				return Ray(result.point,direction,SOURCE::TRANSMISSON);
			else
			{
				direction = Reflect(ray.direction,normal);
				return Ray(result.point,direction,SOURCE::SPECULAR_REFLECT);
			}
		}
		//else survivalContribution *= 1.0/(1.0-transmissonSurvival);
	}
	
	//If reflection : diffuse or specular?
	double specularSurvival;
	if(russianRoulette(num[0]/num[1],specularSurvival))
	{
		//survivalContribution *= 1.0/specularSurvival;
		Vec3& prefectReflectDirection = Reflect(ray.direction,result.normal);
		direction = importanceSampleUpperHemisphere(prefectReflectDirection,attr.shiness);
		return Ray(result.point,direction,SOURCE::SPECULAR_REFLECT);
	}
	else
	{
		//survivalContribution *= 1.0/(1.0-specularSurvival);
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
	int pos1 = obj_file.find_last_of('/');
	int pos2 = obj_file.find_last_of('.');
	string fileName = obj_file.substr(pos1+1,pos2-pos1-1);
	resultPath = "../result/"+fileName+"_spp"+to_string(mcSampleNum)+".bmp";

	TotalTimer loadingModel("loading model");

	if(Parser::parse(obj_file,&scene))
	{
		loadingModel.print();
		scene.init();
		GlutDisplay::setRayTracer(this);
		GlutDisplay::loop();
	}
}

void RayTracer::writeResultImage()
{
	int width = scene.getWidth(),height = scene.getHeight();

	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width,height,24);
	RGBQUAD color;

	for(int y=0;y<height;++y)
	{
		for(int x=0;x<width;++x)
		{
			int index =  y*width*3+x*3;
			color.rgbRed = scene.color[index]*255;
			color.rgbGreen = scene.color[index+1]*255;
			color.rgbBlue = scene.color[index+2]*255;
			FreeImage_SetPixelColor(bitmap,x,y,&color);
		}
	}

	if(FreeImage_Save(FIF_BMP,bitmap,resultPath.c_str())) cout<<"write render image to "<<resultPath<<endl;

	FreeImage_DeInitialise();
}