#ifndef __RAY_H__
#define __RAY_H__
#include "../common/VMath.h"
enum SOURCE{DIRECT, SPECULA_REFLECT,DIFFUSE_REFLECT,TRANSMISSON};

struct Ray
{
	Ray(){};
	Ray(Point3 origin,Vec3 direction,Color3 color=Vec3(1,1,1)):origin(origin),direction(Normalize(direction)),color(color){};
	Point3 getPoint(double time){return origin + direction*time;};


	Point3 origin;
	Vec3 direction;
	Color3 color;
	SOURCE souce;
};
#endif