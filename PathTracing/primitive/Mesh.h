#ifndef __MESH_H__
#define __MESH_H__
#include <vector>
#include "../common/VMath.h"
#include "IPrimitive.h"
#include "../light/ILight.h"
#include "../raytracer/KdTree.h"
using namespace std;

class Mesh;

class MeshTriangle : public IPrimitive,public ILight
{
public:
	MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[]);
	void init();

	bool intersect(Ray& ray,IntersectResult& result);
	AABB getAABB();
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);

	Vec2 getTextureCoordinate(const Vec3& point);
	Vec3 getNormal(const Vec3& point);

	Mesh* mesh;
	int vertI[3];
	int normI[3];
	int texI[3];
private:
	Point3 origin;
	Vec3 dx,dy;
	Vec3 normal;
	int lightSamples;
	Mat4 barycentric;
};

class Mesh : public IPrimitive,public ILight
{
public:
	bool intersect(Ray& ray,IntersectResult& result);
	AABB getAABB();
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);
	void init();

	vector<Point3> vertices;
	vector<Vec3> normals;
	vector<Point2> textures;

	vector<Point3> resizeVertices;
	vector<IPrimitive*> triangleList;
private:
	KdTree kdTree;
	vector<IPrimitive*> lights;
};


#endif