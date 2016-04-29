#include "Camera.h"

Camera::Camera()
{
	lookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	perspective(60, 1.0, 0.01, 2000.0);
}

void Camera::lookAt(double ex, double ey, double ez, double lx, double ly, double lz, double upx, double upy, double upz)
{
	origin = Vec3(ex,ey,ez);
	direction = Vec3(lx-ex,ly-ey,lz-ez);
	up = Vec3(upx,upy,upz);

	right = Normalize(Cross(direction,up));
	up = Normalize(Cross(right,direction));
}

void Camera::perspective(double fov, double aspect, double near, double far)
{
	nearPlane = near;
	farPlane = far;

	height = 2*nearPlane*tan(fov/2*PI/180);
	width = height*aspect;

	calcViewPort(fov,width,height);
}

void Camera::rotate(double angle, const Vec3& axis)
{
	Mat4 mat = RotateMatrix(angle,axis);
	up = mat *  up;
	front = mat * front;
	right = mat * right;
}

void Camera::calcViewPort(double fov,double width,double height)
{
	this->fov = fov;
	this->width = width;
	this->height = height;

	double aspect = height / width;

	view_x = right * 2 * tan(fov * PI / 360);
	view_y = up * 2 * tan(fov * aspect * PI / 360);
	view_z = direction;
}

Ray Camera::getRay(double x,double y)
{
	/*Point3 position = (x-0.5)*width*right+(y-0.5)*height*up + nearPlane * front;
	Vec3 direction = position - origin;
	return Ray(origin,direction);*/

	Vec3& direction = view_z + (x - 0.5f) * view_x
		+ (y-0.5) * view_y;
	return Ray(origin,direction);
}