#include "Mesh.h"

MeshTriangle::MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[])
{
	this->mesh = mesh;
	memcpy(this->vertI,vertI,sizeof(vertI)*3);
	memcpy(this->normI,normI,sizeof(normI)*3);
	memcpy(this->texI,texI,sizeof(texI)*3);
}

void MeshTriangle::init()
{
	Point3& pt1 =mesh->vertices[vertI[0]],&pt2 = mesh->vertices[vertI[1]],&pt3 = mesh->vertices[vertI[2]];
	origin = pt1;
	dx = pt2-pt1;
	dy = pt3-pt1;

	normal = Normalize(Cross(dx,dy));
	barycentric = Mat4(pt1,pt2,pt3);
	barycentric.inverse();
}

AABB MeshTriangle::getAABB()
{
	Point3 low,high;
	Point3& pt1 =mesh->vertices[vertI[0]],&pt2 = mesh->vertices[vertI[1]],&pt3 = mesh->vertices[vertI[2]];

	low.x = min(pt1.x,min(pt2.x,pt3.x));
	low.y = min(pt1.y,min(pt2.y,pt3.y));
	low.z = min(pt1.z,min(pt2.z,pt3.z));

	high.x = max(pt1.x,max(pt2.x,pt3.x));
	high.y = max(pt1.y,max(pt2.y,pt3.y));
	high.z = max(pt1.z,max(pt2.z,pt3.z));

	return AABB(low,high);
}

bool MeshTriangle::intersect(Ray& ray,IntersectResult& result)
{
	double tmp = Dot(normal,ray.direction);

	if(DoubleEquals(tmp,0)) return false;
	//if(DoubleCompare(tmp,0)>0&&ray.source!=SOURCE::TRANSMISSON) return false;

	Vec3 v = origin - ray.origin;

	Vec3 tmp1 = Cross(dy,ray.direction);
	double s = -Dot(tmp1,v)/Dot(tmp1,dx);

	Vec3 tmp2 = Cross(dx,ray.direction);
	double t = -Dot(tmp2,v)/Dot(tmp2,dy);

	double bestTime = Dot(normal,v)/Dot(normal,ray.direction);

	if(DoubleCompare(s,0)>=0&&DoubleCompare(t,0)>=0&&DoubleCompare(s+t,1)<=0&&ray.isWithinBounds(bestTime))
	{
		result.point = ray.getPoint(bestTime);
		result.distance = bestTime;
		result.normal = getNormal(result.point);
		result.primitive = this;
		ray.tMax = bestTime;
		return true;
	}

	return false;
}

Point2 MeshTriangle::getTextureCoordinate(const Point3& point)
{
	Vec3 abg =  barycentric * point;
	return 
		abg.x*mesh->textures[texI[0]] +
		abg.y*mesh->textures[texI[1]] +
		abg.z*mesh->textures[texI[2]];
}

Vec3 MeshTriangle::getNormal(const Vec3& point)
{
	if(normI[0]==-1||normI[1]==-1||normI[2]==-1) return normal;

	Vec3 abg = barycentric * point;
	return Normalize(
		abg.x*(mesh->normals[normI[0]]) +
		abg.y*(mesh->normals[normI[1]]) +
		abg.z*(mesh->normals[normI[2]]));
}

bool Mesh::intersect(Ray& ray,IntersectResult& result)
{
	return kdTree.intersect(ray,result);
}

bool Mesh::shadowRayIntersect(Ray& ray,IntersectResult& result)
{
	return kdTree.shadowRayIntersect(ray,result);
}


AABB Mesh::getAABB()
{
	return kdTree.getAABB();
}

void Mesh::init()
{
	kdTree.build(triangleList);
}