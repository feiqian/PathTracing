#include "KdTree.h"

KdTree::KdTree():low(NULL),high(NULL)
{

}

KdTree::KdTree(vector<IPrimitive*>& primitiveList,int splitAxis /* = AXIS::Z */)
{
	build(primitiveList,splitAxis);
}

void KdTree::build(vector<IPrimitive*>& primitiveList,int splitAxis /* = AXIS::Z */)
{
	this->splitAxis = splitAxis;
	unsigned int length = primitiveList.size();

	if (length == 1) {
		low = primitiveList[0];
		high = NULL;
		box = low->getAABB();
	} else if (length == 2) {
		low = primitiveList[0];
		high = primitiveList[1];
		box = AABB::combine(low->getAABB(), high->getAABB());
	} else {
		vector<IPrimitive*> lowVec;
		vector<IPrimitive*> highVec;
		partition(splitAxis, primitiveList, &lowVec, &highVec);

		low = lowVec.size() > 0 ? new KdTree(lowVec, (splitAxis + 1) % 3) : NULL;
		high = highVec.size() > 0 ? new KdTree(highVec, (splitAxis + 1) % 3) : NULL;

		if (low == NULL)
			box = high->getAABB();
		else if (high == NULL)
			box = low->getAABB();
		else box = AABB::combine(low->getAABB(), high->getAABB());
	}
}

void KdTree::partition(int axis, const vector<IPrimitive*>& all, vector<IPrimitive*>* lowVec, vector<IPrimitive*>* highVec) {

	double minVal, maxVal;
	minVal = DOUBLE_POSITIVE_INFINITY;
	maxVal = DOUBLE_NEGATIVE_INFINITY;

	for(unsigned int i = 0; i < all.size(); i++) {
		AABB bbox = all[i]->getAABB();
		minVal = min(minVal, bbox.minCoordinate(axis));
		maxVal = max(maxVal, bbox.maxCoordinate(axis));
	}

	double pivot = (maxVal + minVal) / 2;

	for (unsigned int i = 0; i < all.size(); i++) {
		AABB bbox = all[i]->getAABB();
		double centerCoord = (bbox.maxCoordinate(axis) + bbox.minCoordinate(axis)) / 2;
		if (centerCoord < pivot)
			lowVec->push_back(all[i]);
		else highVec->push_back(all[i]);
	}

	// Check to see if we failed to make a partition. If so, grab the closest thing
	// that isn't on the other side of the partition and throw it into the empty list.

	// CASE: Everything ended up in highVec
	if (highVec->size() == all.size()) {
		double minCoord = DOUBLE_POSITIVE_INFINITY;
		unsigned int index;
		IPrimitive* obj;
		for (unsigned int i = 0; i < highVec->size(); i++) {
			AABB bbox = (*highVec)[i]->getAABB();
			double centerCoord = (bbox.maxCoordinate(axis) + bbox.minCoordinate(axis)) / 2;
			if (centerCoord < minCoord) {
				minCoord = centerCoord;
				index = i;
				obj = (*highVec)[i];
			}
		}
		highVec->erase(highVec->begin() + index);
		lowVec->push_back(obj);
	}
	// CASE: Everything ended up in lowVec
	else if (lowVec->size() == all.size()) {
		double maxCoord = DOUBLE_NEGATIVE_INFINITY;
		unsigned int index;
		IPrimitive* obj;
		for (unsigned int i = 0; i < lowVec->size(); i++) {
			AABB bbox = (*lowVec)[i]->getAABB();
			double centerCoord = (bbox.maxCoordinate(axis) + bbox.minCoordinate(axis)) / 2;
			if (centerCoord > maxCoord) {
				maxCoord = centerCoord;
				index = i;
				obj = (*lowVec)[i];
			}
		}
		lowVec->erase(lowVec->begin() + index);
		highVec->push_back(obj);
	}
}

bool KdTree::intersect(Ray& ray,IntersectResult& result)
{
	if(!box.intersect(ray,result)) return false;

	bool hit1 = false, hit2 = false;

	if(ray.direction.num[splitAxis]>=0)
	{
		if(low!=NULL)
		{
			hit1 = low->intersect(ray,result);
			ray.setBounds(ray.getLowerBound(),result.distance);
		}
		if(high!=NULL) 
		{
			hit2 = high->intersect(ray,result);
		}
	}
	else
	{
		if(high!=NULL) 
		{
			hit1 = high->intersect(ray,result);
			ray.setBounds(ray.getLowerBound(),result.distance);
		}

		if(low!=NULL)
		{
			hit2 = low->intersect(ray,result);
		}
	}

	return hit1||hit2;
}

AABB KdTree::getAABB()
{
	return box;
}