#ifndef __KD_TREE_H__
#define __KD_TREE_H__
#include <iostream>
#include <vector>
#include "AABB.h"
using namespace std;

enum AXIS
{
	X,Y,Z
};

class KdTree : public IPrimitive
{
public:
	KdTree();
	KdTree(vector<IPrimitive*>& primitiveList,int splitAxis = AXIS::Z);

	void build(vector<IPrimitive*>& primitiveList,int splitAxis = AXIS::Z);
	AABB getAABB();

	bool intersect(Ray& ray,IntersectResult& result);

	static void partition(int axis, const vector<IPrimitive*>& all, vector<IPrimitive*>* lowVec, vector<IPrimitive*>* highVec);
private:
	AABB box;
	int splitAxis;
	IPrimitive* low,*high;
};


#endif