#ifndef __RAY_H__
#define __RAY_H__
#include "../common/VMath.h"
enum SOURCE{NONE,DIRECT,DIFFUSE_REFLECT,SPECULAR_REFLECT,TRANSMISSON};

class Ray
{
public:
	Ray(Point3 origin=Point3(0,0,0),Vec3 direction=Vec3(0,0,0),SOURCE source=SOURCE::NONE,double IOR = 1.0)
		:origin(origin),direction(Normalize(direction)),source(source),IOR(IOR),tMin(EPS),tMax(DOUBLE_POSITIVE_INFINITY){inverseDirection=1.0/this->direction;};
	Point3 getPoint(double time){return origin + direction*time;};

	inline double getLowerBound() { return tMin; }
	inline double getUpperBound() { return tMax; }
	inline bool isWithinBounds(double tVal) { return tVal <= tMax && tVal >= tMin; }
	inline void setBounds(double min, double max) { tMin = min;tMax = max;}
	inline Vec3 getInverseDirection() {return inverseDirection;}

	Point3 origin;
	Vec3 direction;
	SOURCE source;
	double tMin;
	double tMax;
	double IOR;
private:
	Vec3 inverseDirection;
};

#endif