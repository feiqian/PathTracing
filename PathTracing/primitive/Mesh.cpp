#include "Mesh.h"

MeshTriangle::MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[])
{
	this->mesh = mesh;
	memcpy(this->vertI,vertI,sizeof(vertI)*3);
	memcpy(this->normI,normI,sizeof(normI)*3);
	memcpy(this->texI,texI,sizeof(texI)*3);
	lightSamples = 3;
}

void MeshTriangle::init()
{
	Point3& pt1 =mesh->resizeVertices[vertI[0]],&pt2 = mesh->resizeVertices[vertI[1]],&pt3 = mesh->resizeVertices[vertI[2]];
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
	Point3& pt1 =mesh->resizeVertices[vertI[0]],&pt2 = mesh->resizeVertices[vertI[1]],&pt3 = mesh->resizeVertices[vertI[2]];

	low.x = min(pt1.x,min(pt2.x,pt3.x));
	low.y = min(pt1.y,min(pt2.y,pt3.y));
	low.z = min(pt1.z,min(pt2.z,pt3.z));

	high.x = max(pt1.x,max(pt2.x,pt3.x));
	high.y = max(pt1.y,max(pt2.y,pt3.y));
	high.z = max(pt1.z,max(pt2.z,pt3.z));

	return AABB(low,high);
}

Color3 MeshTriangle::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	//TODO 光源强度没有考虑距离因素
	if(attr.emission==Color3::BLACK ||result.primitive==this) return Color3::BLACK;

	Color3 rgb;
	Material& intersectAttr = result.primitive->attr;
	Reflectance& ref = result.primitive->getReflectance(result.point);
	double rate = 1.0 / lightSamples;

	for (int i = 0; i < lightSamples; i++) {
		double sx = (double)rand() / RAND_MAX;
		double sy = (double)rand() / RAND_MAX * (1.0-sx);
		
		Vec3 lightOrigin = origin + dx * sx + dy * sy;
		Vec3&r = lightOrigin - result.point;
		double rr = Length(r);

		if(!scene->isInShadow(Ray(result.point,r),this))
		{
			Vec3 s = Normalize(r);

			if(ref.kd!=Color3::BLACK)
			{
				//calculate the diffuse color
				double mDots = Dot(s,result.normal);
				if(mDots>0.0) rgb+= mDots*ref.kd
					*attr.emission/PI*rate;
			}

			if(ref.ks!=Color3::BLACK)
			{
				//calculate the specular color
				Vec3 v = ray.direction.flip();
				Vec3 h = Normalize(s+v);
				double mDotH = Dot(h,result.normal);
				if(mDotH>0.0) rgb+= pow(mDotH,intersectAttr.shiness)*ref.ks
					*attr.emission
					*(intersectAttr.shiness+1)/(2*PI)*rate;
			}
		}
		else scene->isInShadow(Ray(result.point,r),this);
	}

	return rgb;
}

bool MeshTriangle::intersect(Ray& ray,IntersectResult& result)
{
	double tmp = Dot(normal,ray.direction);

	if(DoubleEquals(tmp,0)) return false;

	bool inside = DoubleCompare(tmp,0)<0?false:true;

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
		result.normal = inside?-getNormal(result.point):getNormal(result.point);
		result.primitive = this;
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
	Vec3 abg = barycentric * point;
	return Normalize(
		abg.x*(normI[0]!=-1?mesh->normals[normI[0]]:normal) +
		abg.y*(normI[1]!=-1?mesh->normals[normI[1]]:normal) +
		abg.z*(normI[2]!=-1?mesh->normals[normI[2]]:normal));
}

bool Mesh::intersect(Ray& ray,IntersectResult& result)
{
	return kdTree.intersect(ray,result);
}

Color3 Mesh::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	Color3 rgb;
	for(int i=0,len=lights.size();i<len;++i)
	{
		rgb += ((MeshTriangle*)lights[i])->render(result,ray,scene);
	}
	return rgb;
}

AABB Mesh::getAABB()
{
	return kdTree.getAABB();
}

void Mesh::init()
{
	for(int i=0,len = triangleList.size();i<len;++i) 
	{
		((MeshTriangle*)triangleList[i])->init();
		if(triangleList[i]->attr.emission!=Color3::BLACK) lights.push_back(triangleList[i]);
	}

	kdTree.build(triangleList);
}