#ifndef __RAY_H__
#define __RAY_H__
#include "../common/VMath.h"
enum SOURCE{NONE,DIRECT, SPECULA_REFLECT,DIFFUSE_REFLECT,TRANSMISSON};

struct Ray
{
	Ray(Point3 origin=Point3(0,0,0),Vec3 direction=Vec3(0,0,0),Color3 color=Vec3(1,1,1))
		:origin(origin),direction(Normalize(direction)),color(color),source(SOURCE::NONE),tMin(EPS),tMax(DOUBLE_POSITIVE_INFINITY){};
	Point3 getPoint(double time){return origin + direction*time;};

	inline double getLowerBound() { return tMin; }
	inline double getUpperBound() { return tMax; }
	inline bool isWithinBounds(double tVal) { return tVal <= tMax && tVal >= tMin; }
	inline void setBounds(double min, double max) { tMin = min;tMax = max;}
	inline Vec3 getInverseDirection() {return 1.0/direction;}

	Point3 origin;
	Vec3 direction;
	Color3 color;
	SOURCE source;

	double tMin;
	double tMax;
};

#endif