#ifndef __INTERSECT_RESULT_H__
#define __INTERSECT_RESULT_H__
#include <stdio.h>
#include "VMath.h"
#include "../primitive/IPrimitive.h"

class IPrimitive;
struct IntersectResult
{
	IntersectResult():primitive(NULL),distance(DOUBLE_POSITIVE_INFINITY){};

	IPrimitive* primitive;
	double distance;
	Point3 point;
	Vec3 normal;
};
#endif