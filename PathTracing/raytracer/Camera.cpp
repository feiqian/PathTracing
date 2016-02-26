#include "Camera.h"

Camera::Camera()
{
	lookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	perspective(90, 1.0, 0.1, 200.0);
}

void Camera::lookAt(double ex, double ey, double ez, double lx, double ly, double lz, double upx, double upy, double upz)
{
	this->origin = Vec3(ex,ey,ez);
	this->center = Vec3(lx,ly,lz);
	this->up = Vec3(upx,upy,upz);

	front = Normalize(center-origin);
	right = Normalize(Cross(front,up));
	up = Normalize(Cross(right,front));
}

void Camera::perspective(double fov, double aspect, double near, double far)
{
	this->fov = fov;
	nearPlane = near;
	farPlane = far;
	this->setAspect(aspect);
}

void Camera::setAspect(double aspect)
{
	if(this->aspect == aspect) return;
	this->aspect = aspect;
	height = 2*nearPlane*tan(fov/2*PI/180);
	width = height*aspect;
}

void Camera::rotate(double angle, const Vec3& axis)
{
	Mat4 mat = RotateMatrix(angle,axis);
	up = mat *  up;
	front = mat * front;
	right = mat * right;
}

Ray Camera::getRay(double x,double y)
{
	Point3 position = (x-0.5)*width*right+(y-0.5)*height*up + nearPlane * front;
	Vec3 direction = position - origin;
	return Ray(origin,direction);
}