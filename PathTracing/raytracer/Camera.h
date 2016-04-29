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
	void lookAt(double ex, double ey, double ez, double lx, double ly, double lz, double upx, double upy, double upz);
	void perspective(double fov, double aspect, double near, double far);
	void calcViewPort(double fov,double width,double height);
	void rotate(double angle, const Vec3& axis);

	double width,height;//�ӿڿ��
	double nearPlane,farPlane;
private:
	Point3 origin;
	Point3 direction;

	double fov;//��ֱ����� filed of view
	double rotateX,rotateY;
	Vec3 front,up,right;
	Vec3 view_x,view_y,view_z;
};

#endif