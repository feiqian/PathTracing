#ifndef __MESH_H__
#define __MESH_H__
#include <vector>
#include "../common/VMath.h"
#include "IPrimitive.h"
#include "../light/ILight.h"
using namespace std;

class Mesh;

class MeshTriangle : public IPrimitive,public ILight
{
public:
	MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[]);
	void resize();

	IntersectResult intersect(Ray& ray);
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);

	Mesh* mesh;
	int vertI[3];
	int normI[3];
	int texI[3];
private:
	Point3 origin;
	Vec3 dx,dy;
	Vec3 normal;
	int lightSamples;
};

class Mesh : public IPrimitive,public ILight
{
public:
	IntersectResult intersect(Ray& ray);
	Color3 render(IntersectResult& result,Ray& ray,Scene* scene);

	vector<MeshTriangle> triangleList;

	vector<Point3> vertices;
	vector<Vec3> normals;
	vector<Point2> textures;

	vector<Point3> resizeVertices;
};


#endif