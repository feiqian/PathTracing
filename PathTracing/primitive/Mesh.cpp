#include "Mesh.h"

MeshTriangle::MeshTriangle(Mesh* mesh,int vertI[], int normI[], int texI[])
{
	this->mesh = mesh;
	memcpy(this->vertI,vertI,sizeof(vertI)*3);
	memcpy(this->normI,normI,sizeof(normI)*3);
	memcpy(this->texI,texI,sizeof(texI)*3);
	lightSamples = 1;
}

void MeshTriangle::resize()
{
	Point3& pt1 =mesh->resizeVertices[vertI[0]],&pt2 = mesh->resizeVertices[vertI[1]],&pt3 = mesh->resizeVertices[vertI[2]];
	origin = pt1;
	dx = pt2-pt1;
	dy = pt3-pt1;
	normal = Normalize(Cross(dx,dy));
}

Color3 MeshTriangle::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	if(attr.emission==Color3::BLACK ||result.primitive==this) return Color3::BLACK;

	Color3 rgb;
	Material& intersectAttr = result.primitive->attr;
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

			if(intersectAttr.kd!=Color3::BLACK)
			{
				//calculate the diffuse color
				double mDots = Dot(s,result.normal);
				if(mDots>0.0) rgb+= mDots*intersectAttr.kd*intersectAttr.ka
					*attr.emission/PI;
			}

			if(intersectAttr.ks!=Color3::BLACK)
			{
				//calculate the specular color
				Vec3 v = ray.direction.flip();
				Vec3 h = Normalize(s+v);
				double mDotH = Dot(h,result.normal);
				if(mDotH>0.0) rgb+= pow(mDotH,intersectAttr.shiness)*intersectAttr.ks
					*attr.emission
					*(intersectAttr.shiness+1)/(2*PI);
			}
		}
		else scene->isInShadow(Ray(result.point,r),this);
	}

	return rgb;
}

IntersectResult MeshTriangle::intersect(Ray& ray)
{
	IntersectResult result;

	double tmp = Dot(normal,ray.direction);

	if(DoubleEquals(tmp,0)) return result;

	bool inside = DoubleCompare(tmp,0)<0?false:true;

	Vec3 v = origin - ray.origin;

	Vec3 tmp1 = Cross(dy,ray.direction);
	double s = -Dot(tmp1,v)/Dot(tmp1,dx);

	Vec3 tmp2 = Cross(dx,ray.direction);
	double t = -Dot(tmp2,v)/Dot(tmp2,dy);

	double bestTime = Dot(normal,v)/Dot(normal,ray.direction);

	if(DoubleCompare(s,0)>=0&&DoubleCompare(t,0)>=0&&DoubleCompare(s+t,1)<=0&&DoubleCompare(bestTime,0)>0)
	{
		result.point = ray.getPoint(bestTime);
		result.distance = bestTime;
		result.normal = inside?-normal:normal;
		result.primitive = this;
	}

	return result;
}

IntersectResult Mesh::intersect(Ray& ray)
{
	IntersectResult best;
	best.distance = 1e20;

	for(int i=0,len=triangleList.size();i<len;++i)
	{
		IntersectResult& result = triangleList[i].intersect(ray);
		if(result.isHit()&&result.distance<best.distance) best = result;
	}
	return best;
}

Color3 Mesh::render(IntersectResult& result,Ray& ray,Scene* scene)
{
	Color3 rgb;
	for(int i=0,len=triangleList.size();i<len;++i)
	{
		rgb += triangleList[i].render(result,ray,scene);
	}
	return rgb;
}