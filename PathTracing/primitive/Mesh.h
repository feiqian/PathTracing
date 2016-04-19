#ifndef __MESH_H__
#define __MESH_H__
#include <vector>
#include "../common/VMath.h"
#include "IPrimitive.h"
#include "Triangle.h"
using namespace std;

class Mesh;

class MeshTriangle : public Triangle
{
public:
	MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[]);

	Mesh* mesh;
	int vertI[3];
	int normI[3];
	int texI[3];
};


class Mesh : public IPrimitive
{
public:
	IntersectResult intersect(Ray& ray);

	vector<MeshTriangle> triangleList;

	vector<Point3> vertices;
	vector<Vec3> normals;
	vector<Point2> textures;
};


#endif