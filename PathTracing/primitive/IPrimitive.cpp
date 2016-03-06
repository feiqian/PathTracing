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