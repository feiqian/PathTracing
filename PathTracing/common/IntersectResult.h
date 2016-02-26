#ifndef __INTERSECT_RESULT_H__
#define __INTERSECT_RESULT_H__
#include <stdio.h>
#include "VMath.h"
#include "../primitive/IPrimitive.h"

class IPrimitive;
struct IntersectResult
{
	IntersectResult():primitive(NULL),distance(0.0){};
	bool isHit(){return this->primitive!=NULL;};

	IPrimitive* primitive;
	double distance;
	Point3 point;
	Vec3 normal;
};
#endif