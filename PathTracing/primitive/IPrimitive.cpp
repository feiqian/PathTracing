#include "IPrimitive.h"

Ray IPrimitive::getTransformRay(Ray& ray)
{
	Ray transformRay;
	transformRay.origin = modelMatrixInverse * Vec4(ray.origin,1.0);
	transformRay.direction = modelMatrixInverse * ray.direction;
	return transformRay;
}

Vec3 IPrimitive::getTransformNormal(Vec3 normal)
{
	return  Normalize(modelMatrixInverse.transpose()*normal);
}

void IPrimitive::translate(double x,double y,double z)
{
	modelMatrix = TranslationMatrix(x,y,z) * modelMatrix;
	modelMatrixInverse = modelMatrixInverse * TranslationMatrixInverse(x,y,z);
}

void IPrimitive::rotate(double angle, const Vec3& axis)
{
	modelMatrix = RotateMatrix(angle,axis) * modelMatrix;
	modelMatrixInverse = modelMatrixInverse * RotateMatrixInverse(angle,axis);
}

void IPrimitive::scale(double x,double y,double z)
{
	modelMatrix = ScaleMatrix(x,y,z) * modelMatrix;
	modelMatrixInverse = modelMatrixInverse * ScaleMatrixInverse(x,y,z);
}

Point2 IPrimitive::getTextureCoordinate(const Point3& point)
{
	return Point2(point.x,point.y);
}

Reflectance IPrimitive::getReflectance(const Vec3& point)
{
	if(attr.kaTexture==NULL&&attr.kdTexture==NULL&&attr.ksTexture==NULL) return attr.ref;
	Reflectance returnRef;
	Point2 uv = getTextureCoordinate(point);

	returnRef.ka = attr.kaTexture!=NULL?attr.ref.ka*attr.kaTexture->getColor(uv):attr.ref.ka;
	returnRef.kd = attr.kdTexture!=NULL?attr.ref.kd*attr.kdTexture->getColor(uv):attr.ref.kd;
	returnRef.ks = attr.ksTexture!=NULL?attr.ref.ks*attr.ksTexture->getColor(uv):attr.ref.ks;

	return returnRef;
}

bool IPrimitive::shadowRayIntersect(Ray& ray,IntersectResult& result)
{
	return intersect(ray,result);
}