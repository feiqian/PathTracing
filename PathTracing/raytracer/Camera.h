#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <vector>
#include <cmath>
#include "../common/VMath.h"
#include "Ray.h"

class Camera
{
public:
	Camera();
	Ray getRay(double x,double y);
	void setAspect(double aspect);
	void lookAt(double ex, double ey, double ez, double lx, double ly, double lz, double upx, double upy, double upz);
	void perspective(double fov, double aspect, double near, double far);
	void rotate(double angle, const Vec3& axis);

	double width,height;//�ӿڿ��
	double nearPlane,farPlane;
private:
	Point3 origin;
	Point3 center;

	double fov;//��ֱ����� filed of view
	double aspect;

	double rotateX,rotateY;
	Vec3 front,up,right;
};

#endif