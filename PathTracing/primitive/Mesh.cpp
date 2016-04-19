#include "Mesh.h"

MeshTriangle::MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[]):
	Triangle(mesh->vertices[vertI[0]],mesh->vertices[vertI[1]],mesh->vertices[vertI[2]])
{
	this->mesh = mesh;
	memcpy(this->vertI,vertI,sizeof(vertI)*3);
	memcpy(this->normI,vertI,sizeof(normI)*3);
	memcpy(this->texI,vertI,sizeof(texI)*3);
}


IntersectResult Mesh::intersect(Ray& ray)
{
	IntersectResult result;
	for(int i=0,len=triangleList.size();i<len;++i)
	{
		result = triangleList[i].intersect(ray);
		if(result.isHit()) break;
	}
	return result;
}