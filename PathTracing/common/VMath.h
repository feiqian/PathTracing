#ifndef __V_MATH_H__
#define __V_MATH_H__
#define _USE_MATH_DEFINES
#include <math.h>
const double EPS = 1e-5;

const double DOUBLE_POSITIVE_INFINITY = 1e20;
const double DOUBLE_NEGATIVE_INFINITY = -1e20;

class Vec2;
class Vec3;
typedef Vec3 Color3;
typedef Vec3 Point3;
typedef Vec2 Point2;
static const double PI = acos(-1.0);

inline bool DoubleEquals(double a, double b, double eps = EPS)
{
	if (fabs(a - b) < eps) {
		return true;
	}
	return false;
}
inline int DoubleCompare(double a, double b, double eps = EPS)
{
	if (DoubleEquals(a, b, eps)) {
		return 0;
	}
	if (a < b) {
		return -1;
	}
	return 1;
}

class Vec2
{
public:
	union {
		struct { double x, y; };
		struct { double s, t; };
		struct { double r, g; };
		double num[2];
	};
	Vec2() : x(0.0f), y(0.0f) { }
	~Vec2() { }
	Vec2(double num) : x(num), y(num) { }
	Vec2(double x, double y) : x(x), y(y) { }
	Vec2(const Vec2 &u) : x(u.x), y(u.y) { }
	inline Vec2& operator = (const Vec2 &u) { x = u.x; y = u.y; return *this; }
	inline Vec2 operator - () { return Vec2(-x, -y); }
	inline Vec2& operator += (double num) { x += num; y += num; return *this; }
	inline Vec2& operator += (const Vec2 &u) { x += u.x; y += u.y; return *this; }
	inline Vec2& operator -= (double num) { x -= num; y -= num; return *this; }
	inline Vec2& operator -= (const Vec2 &u) { x -= u.x; y -= u.y; return *this; }
	inline Vec2& operator *= (double num) { x *= num; y *= num; return *this; }
	inline Vec2& operator *= (const Vec2 &u) { x *= u.x; y *= u.y; return *this; }
	inline Vec2& operator /= (double num) { x /= num; y /= num; return *this; }
	inline Vec2& operator /= (const Vec2 &u) { x /= u.x; y /= u.y; return *this; }
	inline bool operator == (const Vec2 &u) { return DoubleEquals(x, u.x) && DoubleEquals(y, u.y); }
	inline bool operator != (const Vec2 &u) { return !(DoubleEquals(x, u.x) && DoubleEquals(y, u.y)); }
	friend inline Vec2 operator + (const Vec2 &u, double num) { return Vec2(u.x + num, u.y + num); }
	friend inline Vec2 operator + (double num, const Vec2 &u) { return Vec2(num + u.x, num + u.y); }
	friend inline Vec2 operator + (const Vec2 &u, const Vec2 &v) { return Vec2(u.x + v.x, u.y + v.y); }
	friend inline Vec2 operator - (const Vec2 &u, double num) { return Vec2(u.x - num, u.y - num); }
	friend inline Vec2 operator - (double num, const Vec2 &u) { return Vec2(num - u.x, num - u.y); }
	friend inline Vec2 operator - (const Vec2 &u, const Vec2 &v) { return Vec2(u.x - v.x, u.y - v.y); }
	friend inline Vec2 operator * (const Vec2 &u, double num) { return Vec2(u.x * num, u.y * num); }
	friend inline Vec2 operator * (double num, const Vec2 &u) { return Vec2(num * u.x, num * u.y); }
	friend inline Vec2 operator * (const Vec2 &u, const Vec2 &v) { return Vec2(u.x * v.x, u.y * v.y); }
	friend inline Vec2 operator / (const Vec2 &u, double num) { return Vec2(u.x / num, u.y / num); }
	friend inline Vec2 operator / (double num, const Vec2 &u) { return Vec2(num / u.x, num / u.y); }
	friend inline Vec2 operator / (const Vec2 &u, const Vec2 &v) { return Vec2(u.x / v.x, u.y / v.y); }
};

// ----------------------------------------------------------------------------------------------------------------------------

class Vec3
{
public:
	union {
		struct { double x, y, z; };
		struct { double s, t, p; };
		struct { double r, g, b; };
		double num[3];
	};
	static const Vec3 NONE,BLACK,WHITE;

	Vec3() : x(0.0f), y(0.0f), z(0.0f) { }
	~Vec3() { }
	Vec3(double num) : x(num), y(num), z(num) { }
	Vec3(double x, double y, double z) : x(x), y(y), z(z) { }
	Vec3(const Vec2 &u, double z) : x(u.x), y(u.y), z(z) { }
	Vec3(const Vec3 &u) : x(u.x), y(u.y), z(u.z) { }
	Vec3 multiple(const Vec3 &u){return Vec3(x*u.x,y*u.y, z*u.z);};
	Vec3 flip(){return Vec3(-x, -y, -z);};
	inline Vec3& operator = (const Vec3 &u) { x = u.x; y = u.y; z = u.z; return *this; }
	inline Vec3 operator - () { return Vec3(-x, -y, -z); }
	inline double* operator & () { return (double*)this; }
	inline operator Vec2 () { return *(Vec2*)this; }
	inline Vec3& operator += (double num) { x += num; y += num; z += num; return *this; }
	inline Vec3& operator += (const Vec3 &u) { x += u.x; y += u.y; z += u.z; return *this; }
	inline Vec3& operator -= (double num) { x -= num; y -= num; z -= num; return *this; }
	inline Vec3& operator -= (const Vec3 &u) { x -= u.x; y -= u.y; z -= u.z; return *this; }
	inline Vec3& operator *= (double num) { x *= num; y *= num; z *= num; return *this; }
	inline Vec3& operator *= (const Vec3 &u) { x *= u.x; y *= u.y; z *= u.z; return *this; }
	inline Vec3& operator /= (double num) { x /= num; y /= num; z /= num; return *this; }
	inline Vec3& operator /= (const Vec3 &u) { x /= u.x; y /= u.y; z /= u.z; return *this; }
	inline bool operator == (const Vec3 &u) { return DoubleEquals(x, u.x) && DoubleEquals(y, u.y) && DoubleEquals(z, u.z); }
	inline bool operator != (const Vec3 &u) { return !(DoubleEquals(x, u.x) && DoubleEquals(y, u.y) && DoubleEquals(z, u.z)); }
	friend inline Vec3 operator + (const Vec3 &u, double num) { return Vec3(u.x + num, u.y + num, u.z + num); }
	friend inline Vec3 operator + (double num, const Vec3 &u) { return Vec3(num + u.x, num + u.y, num + u.z); }
	friend inline Vec3 operator + (const Vec3 &u, const Vec3 &v) { return Vec3(u.x + v.x, u.y + v.y, u.z + v.z); }
	friend inline Vec3 operator - (const Vec3 &u, double num) { return Vec3(u.x - num, u.y - num, u.z - num); }
	friend inline Vec3 operator - (double num, const Vec3 &u) { return Vec3(num - u.x, num - u.y, num - u.z); }
	friend inline Vec3 operator - (const Vec3 &u, const Vec3 &v) { return Vec3(u.x - v.x, u.y - v.y, u.z - v.z); }
	friend inline Vec3 operator * (const Vec3 &u, double num) { return Vec3(u.x * num, u.y * num, u.z * num); }
	friend inline Vec3 operator * (double num, const Vec3 &u) { return Vec3(num * u.x, num * u.y, num * u.z); }
	friend inline Vec3 operator * (const Vec3 &u, const Vec3 &v) { return Vec3(u.x * v.x, u.y * v.y, u.z * v.z); }
	friend inline Vec3 operator / (const Vec3 &u, double num) { return Vec3(u.x / num, u.y / num, u.z / num); }
	friend inline Vec3 operator / (double num, const Vec3 &u) { return Vec3(num / u.x, num / u.y, num / u.z); }
	friend inline Vec3 operator / (const Vec3 &u, const Vec3 &v) { return Vec3(u.x / v.x, u.y / v.y, u.z / v.z); }
};


// ----------------------------------------------------------------------------------------------------------------------------

class Vec4
{
public:
	union {
		struct { double x, y, z, w; };
		struct { double s, t, p, q; };
		struct { double r, g, b, a; };
		double num[4];
	};
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
	~Vec4() { }
	Vec4(double num) : x(num), y(num), z(num), w(num) { }
	Vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) { }
	Vec4(const Vec2 &u, double z, double w) : x(u.x), y(u.y), z(z), w(w) { }
	Vec4(const Vec3 &u, double w) : x(u.x), y(u.y), z(u.z), w(w) { }
	Vec4(const Vec4 &u) : x(u.x), y(u.y), z(u.z), w(u.w) { }
	inline Vec4& operator = (const Vec4 &u) { x = u.x; y = u.y; z = u.z; w = u.w; return *this; }
	inline Vec4 operator - () { return Vec4(-x, -y, -z, -w); }
	inline double* operator & () { return (double*)this; }
	inline operator Vec2 () { return *(Vec2*)this; }
	inline operator Vec3 () { return *(Vec3*)this; }
	inline Vec4& operator += (double num) { x += num; y += num; z += num; w += num; return *this; }
	inline Vec4& operator += (const Vec4 &u) { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }
	inline Vec4& operator -= (double num) { x -= num; y -= num; z -= num; w -= num; return *this; }
	inline Vec4& operator -= (const Vec4 &u) { x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this; }
	inline Vec4& operator *= (double num) { x *= num; y *= num; z *= num; w *= num; return *this; }
	inline Vec4& operator *= (const Vec4 &u) { x *= u.x; y *= u.y; z *= u.z; w *= u.w; return *this; }
	inline Vec4& operator /= (double num) { x /= num; y /= num; z /= num; w /= num; return *this; }
	inline Vec4& operator /= (const Vec4 &u) { x /= u.x; y /= u.y; z /= u.z; w /= u.w; return *this; }
	inline bool operator == (const Vec4 &u) { return DoubleEquals(x, u.x) && DoubleEquals(y, u.y) && DoubleEquals(z, u.z) && DoubleEquals(w, u.w); }
	inline bool operator != (const Vec4 &u) { return !(DoubleEquals(x, u.x) && DoubleEquals(y, u.y) && DoubleEquals(z, u.z) && DoubleEquals(w, u.w)); }
	friend inline Vec4 operator + (const Vec4 &u, double num) { return Vec4(u.x + num, u.y + num, u.z + num, u.w + num); }
	friend inline Vec4 operator + (double num, const Vec4 &u) { return Vec4(num + u.x, num + u.y, num + u.z, num + u.w); }
	friend inline Vec4 operator + (const Vec4 &u, const Vec4 &v) { return Vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); }
	friend inline Vec4 operator - (const Vec4 &u, double num) { return Vec4(u.x - num, u.y - num, u.z - num, u.w - num); }
	friend inline Vec4 operator - (double num, const Vec4 &u) { return Vec4(num - u.x, num - u.y, num - u.z, num - u.w); }
	friend inline Vec4 operator - (const Vec4 &u, const Vec4 &v) { return Vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); }
	friend inline Vec4 operator * (const Vec4 &u, double num) { return Vec4(u.x * num, u.y * num, u.z * num, u.w * num); }
	friend inline Vec4 operator * (double num, const Vec4 &u) { return Vec4(num * u.x, num * u.y, num * u.z, num * u.w); }
	friend inline Vec4 operator * (const Vec4 &u, const Vec4 &v) { return Vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w); }
	friend inline Vec4 operator / (const Vec4 &u, double num) { return Vec4(u.x / num, u.y / num, u.z / num, u.w / num); }
	friend inline Vec4 operator / (double num, const Vec4 &u) { return Vec4(num / u.x, num / u.y, num / u.z, num / u.w); }
	friend inline Vec4 operator / (const Vec4 &u, const Vec4 &v) { return Vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w); }
};

// ----------------------------------------------------------------------------------------------------------------------------

class Mat4
{
public:
	double mat[16];
	Mat4()
	{
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	Mat4(const Vec3& v0, const Vec3& v1, const Vec3& v2)
	{
		mat[0] = v0.x; mat[4] = v1.x; mat[8] = v2.x; mat[12] = 0.0f;
		mat[1] = v0.y; mat[5] = v1.y; mat[9] = v2.y; mat[13] = 0.0f;
		mat[2] = v0.z; mat[6] = v1.z; mat[10] = v2.z; mat[14] = 0.0f;
		mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	}
	~Mat4() { }
	Mat4(const Mat4 &Mat4)
	{
		for (int i = 0; i < 16; i++) {
			mat[i] = Mat4.mat[i];
		}
	}
	Mat4 transpose()
	{
		Mat4 Matrix;
		Matrix[0] = mat[0];
		Matrix[1] = mat[4];
		Matrix[2] = mat[8];
		Matrix[3] = mat[12];
		Matrix[4] = mat[1];
		Matrix[5] = mat[5];
		Matrix[6] = mat[9];
		Matrix[7] = mat[13];
		Matrix[8] = mat[2];
		Matrix[9] = mat[6];
		Matrix[10] = mat[10];
		Matrix[11] = mat[14];
		Matrix[12] = mat[3];
		Matrix[13] = mat[7];
		Matrix[14] = mat[11];
		Matrix[15] = mat[15];
		return Matrix;
	}

	bool inverse()
	{
		double inv[16], det;
		int i;

		inv[0] = mat[5]  * mat[10] * mat[15] - 
			mat[5]  * mat[11] * mat[14] - 
			mat[9]  * mat[6]  * mat[15] + 
			mat[9]  * mat[7]  * mat[14] +
			mat[13] * mat[6]  * mat[11] - 
			mat[13] * mat[7]  * mat[10];

		inv[4] = -mat[4]  * mat[10] * mat[15] + 
			mat[4]  * mat[11] * mat[14] + 
			mat[8]  * mat[6]  * mat[15] - 
			mat[8]  * mat[7]  * mat[14] - 
			mat[12] * mat[6]  * mat[11] + 
			mat[12] * mat[7]  * mat[10];

		inv[8] = mat[4]  * mat[9] * mat[15] - 
			mat[4]  * mat[11] * mat[13] - 
			mat[8]  * mat[5] * mat[15] + 
			mat[8]  * mat[7] * mat[13] + 
			mat[12] * mat[5] * mat[11] - 
			mat[12] * mat[7] * mat[9];

		inv[12] = -mat[4]  * mat[9] * mat[14] + 
			mat[4]  * mat[10] * mat[13] +
			mat[8]  * mat[5] * mat[14] - 
			mat[8]  * mat[6] * mat[13] - 
			mat[12] * mat[5] * mat[10] + 
			mat[12] * mat[6] * mat[9];

		inv[1] = -mat[1]  * mat[10] * mat[15] + 
			mat[1]  * mat[11] * mat[14] + 
			mat[9]  * mat[2] * mat[15] - 
			mat[9]  * mat[3] * mat[14] - 
			mat[13] * mat[2] * mat[11] + 
			mat[13] * mat[3] * mat[10];

		inv[5] = mat[0]  * mat[10] * mat[15] - 
			mat[0]  * mat[11] * mat[14] - 
			mat[8]  * mat[2] * mat[15] + 
			mat[8]  * mat[3] * mat[14] + 
			mat[12] * mat[2] * mat[11] - 
			mat[12] * mat[3] * mat[10];

		inv[9] = -mat[0]  * mat[9] * mat[15] + 
			mat[0]  * mat[11] * mat[13] + 
			mat[8]  * mat[1] * mat[15] - 
			mat[8]  * mat[3] * mat[13] - 
			mat[12] * mat[1] * mat[11] + 
			mat[12] * mat[3] * mat[9];

		inv[13] = mat[0]  * mat[9] * mat[14] - 
			mat[0]  * mat[10] * mat[13] - 
			mat[8]  * mat[1] * mat[14] + 
			mat[8]  * mat[2] * mat[13] + 
			mat[12] * mat[1] * mat[10] - 
			mat[12] * mat[2] * mat[9];

		inv[2] = mat[1]  * mat[6] * mat[15] - 
			mat[1]  * mat[7] * mat[14] - 
			mat[5]  * mat[2] * mat[15] + 
			mat[5]  * mat[3] * mat[14] + 
			mat[13] * mat[2] * mat[7] - 
			mat[13] * mat[3] * mat[6];

		inv[6] = -mat[0]  * mat[6] * mat[15] + 
			mat[0]  * mat[7] * mat[14] + 
			mat[4]  * mat[2] * mat[15] - 
			mat[4]  * mat[3] * mat[14] - 
			mat[12] * mat[2] * mat[7] + 
			mat[12] * mat[3] * mat[6];

		inv[10] = mat[0]  * mat[5] * mat[15] - 
			mat[0]  * mat[7] * mat[13] - 
			mat[4]  * mat[1] * mat[15] + 
			mat[4]  * mat[3] * mat[13] + 
			mat[12] * mat[1] * mat[7] - 
			mat[12] * mat[3] * mat[5];

		inv[14] = -mat[0]  * mat[5] * mat[14] + 
			mat[0]  * mat[6] * mat[13] + 
			mat[4]  * mat[1] * mat[14] - 
			mat[4]  * mat[2] * mat[13] - 
			mat[12] * mat[1] * mat[6] + 
			mat[12] * mat[2] * mat[5];

		inv[3] = -mat[1] * mat[6] * mat[11] + 
			mat[1] * mat[7] * mat[10] + 
			mat[5] * mat[2] * mat[11] - 
			mat[5] * mat[3] * mat[10] - 
			mat[9] * mat[2] * mat[7] + 
			mat[9] * mat[3] * mat[6];

		inv[7] = mat[0] * mat[6] * mat[11] - 
			mat[0] * mat[7] * mat[10] - 
			mat[4] * mat[2] * mat[11] + 
			mat[4] * mat[3] * mat[10] + 
			mat[8] * mat[2] * mat[7] - 
			mat[8] * mat[3] * mat[6];

		inv[11] = -mat[0] * mat[5] * mat[11] + 
			mat[0] * mat[7] * mat[9] + 
			mat[4] * mat[1] * mat[11] - 
			mat[4] * mat[3] * mat[9] - 
			mat[8] * mat[1] * mat[7] + 
			mat[8] * mat[3] * mat[5];

		inv[15] = mat[0] * mat[5] * mat[10] - 
			mat[0] * mat[6] * mat[9] - 
			mat[4] * mat[1] * mat[10] + 
			mat[4] * mat[2] * mat[9] + 
			mat[8] * mat[1] * mat[6] - 
			mat[8] * mat[2] * mat[5];

		det = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			mat[i] = inv[i] * det;

		return true;
	}

	Mat4& operator = (const Mat4 &Mat4)
	{
		for (int i = 0; i < 16; i++) {
			mat[i] = Mat4.mat[i];
		}
		return *this;
	}
	inline double& operator [] (int Index)
	{
		return mat[Index];
	}
	inline double* operator & ()
	{
		return (double*)this;
	}
	friend inline Mat4 operator * (const Mat4 &Matrix1, const Mat4 &Matrix2)
	{
		Mat4 Matrix3;

		Matrix3.mat[0] = Matrix1.mat[0] * Matrix2.mat[0] + Matrix1.mat[4] * Matrix2.mat[1] + Matrix1.mat[8] * Matrix2.mat[2] + Matrix1.mat[12] * Matrix2.mat[3];
		Matrix3.mat[1] = Matrix1.mat[1] * Matrix2.mat[0] + Matrix1.mat[5] * Matrix2.mat[1] + Matrix1.mat[9] * Matrix2.mat[2] + Matrix1.mat[13] * Matrix2.mat[3];
		Matrix3.mat[2] = Matrix1.mat[2] * Matrix2.mat[0] + Matrix1.mat[6] * Matrix2.mat[1] + Matrix1.mat[10] * Matrix2.mat[2] + Matrix1.mat[14] * Matrix2.mat[3];
		Matrix3.mat[3] = Matrix1.mat[3] * Matrix2.mat[0] + Matrix1.mat[7] * Matrix2.mat[1] + Matrix1.mat[11] * Matrix2.mat[2] + Matrix1.mat[15] * Matrix2.mat[3];

		Matrix3.mat[4] = Matrix1.mat[0] * Matrix2.mat[4] + Matrix1.mat[4] * Matrix2.mat[5] + Matrix1.mat[8] * Matrix2.mat[6] + Matrix1.mat[12] * Matrix2.mat[7];
		Matrix3.mat[5] = Matrix1.mat[1] * Matrix2.mat[4] + Matrix1.mat[5] * Matrix2.mat[5] + Matrix1.mat[9] * Matrix2.mat[6] + Matrix1.mat[13] * Matrix2.mat[7];
		Matrix3.mat[6] = Matrix1.mat[2] * Matrix2.mat[4] + Matrix1.mat[6] * Matrix2.mat[5] + Matrix1.mat[10] * Matrix2.mat[6] + Matrix1.mat[14] * Matrix2.mat[7];
		Matrix3.mat[7] = Matrix1.mat[3] * Matrix2.mat[4] + Matrix1.mat[7] * Matrix2.mat[5] + Matrix1.mat[11] * Matrix2.mat[6] + Matrix1.mat[15] * Matrix2.mat[7];

		Matrix3.mat[8] = Matrix1.mat[0] * Matrix2.mat[8] + Matrix1.mat[4] * Matrix2.mat[9] + Matrix1.mat[8] * Matrix2.mat[10] + Matrix1.mat[12] * Matrix2.mat[11];
		Matrix3.mat[9] = Matrix1.mat[1] * Matrix2.mat[8] + Matrix1.mat[5] * Matrix2.mat[9] + Matrix1.mat[9] * Matrix2.mat[10] + Matrix1.mat[13] * Matrix2.mat[11];
		Matrix3.mat[10] = Matrix1.mat[2] * Matrix2.mat[8] + Matrix1.mat[6] * Matrix2.mat[9] + Matrix1.mat[10] * Matrix2.mat[10] + Matrix1.mat[14] * Matrix2.mat[11];
		Matrix3.mat[11] = Matrix1.mat[3] * Matrix2.mat[8] + Matrix1.mat[7] * Matrix2.mat[9] + Matrix1.mat[11] * Matrix2.mat[10] + Matrix1.mat[15] * Matrix2.mat[11];

		Matrix3.mat[12] = Matrix1.mat[0] * Matrix2.mat[12] + Matrix1.mat[4] * Matrix2.mat[13] + Matrix1.mat[8] * Matrix2.mat[14] + Matrix1.mat[12] * Matrix2.mat[15];
		Matrix3.mat[13] = Matrix1.mat[1] * Matrix2.mat[12] + Matrix1.mat[5] * Matrix2.mat[13] + Matrix1.mat[9] * Matrix2.mat[14] + Matrix1.mat[13] * Matrix2.mat[15];
		Matrix3.mat[14] = Matrix1.mat[2] * Matrix2.mat[12] + Matrix1.mat[6] * Matrix2.mat[13] + Matrix1.mat[10] * Matrix2.mat[14] + Matrix1.mat[14] * Matrix2.mat[15];
		Matrix3.mat[15] = Matrix1.mat[3] * Matrix2.mat[12] + Matrix1.mat[7] * Matrix2.mat[13] + Matrix1.mat[11] * Matrix2.mat[14] + Matrix1.mat[15] * Matrix2.mat[15];

		return Matrix3;
	}
	friend inline Vec2 operator * (const Mat4 &Mat4, const Vec2 &Vector)
	{
		return Mat4 * Vec4(Vector, 0.0f, 0.0f);
	}
	friend inline Vec3 operator * (const Mat4 &Mat4, const Vec3 &Vector)
	{
		return Mat4 * Vec4(Vector, 0.0f);
	}
	friend inline Vec4 operator * (const Mat4 &Mat4, const Vec4 &Vector)
	{
		Vec4 v;
		v.x = Mat4.mat[0] * Vector.x + Mat4.mat[4] * Vector.y + Mat4.mat[8] * Vector.z + Mat4.mat[12] * Vector.w;
		v.y = Mat4.mat[1] * Vector.x + Mat4.mat[5] * Vector.y + Mat4.mat[9] * Vector.z + Mat4.mat[13] * Vector.w;
		v.z = Mat4.mat[2] * Vector.x + Mat4.mat[6] * Vector.y + Mat4.mat[10] * Vector.z + Mat4.mat[14] * Vector.w;
		v.w = Mat4.mat[3] * Vector.x + Mat4.mat[7] * Vector.y + Mat4.mat[11] * Vector.z + Mat4.mat[15] * Vector.w;
		return v;
	}
};

inline Vec2 Normalize(const Vec2 &u)
{
	return u * (1.0f / sqrt(u.x * u.x + u.y * u.y));
}
inline Vec3 Normalize(const Vec3 &u)
{
	return u * (1.0f / sqrt(u.x * u.x + u.y * u.y + u.z * u.z));
}
// ----------------------------------------------------------------------------------------------------------------------------

inline double Dot(const Vec2 &u, const Vec2 &v)
{
	return u.x * v.x + u.y * v.y;
}
inline double Length(const Vec2 &u)
{
	return sqrt(u.x * u.x + u.y * u.y);
}
inline double Length2(const Vec2 &u)
{
	return u.x * u.x + u.y * u.y;
}
// ----------------------------------------------------------------------------------------------------------------------------

inline Vec3 Cross(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}
inline double Dot(const Vec3 &u, const Vec3 &v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline double Length(const Vec3 &u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}
inline double Length2(const Vec3 &u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}
inline Vec3 Mix(const Vec3 &u, const Vec3 &v, double a)
{
	return u * (1.0f - a) + v * a;
}
inline Vec3 Reflect(const Vec3 &i, const Vec3 &n)
{
	return i - 2.0f * Dot(n, i) * n;
}
inline bool Refract(const Vec3 &i, const Vec3 &n, double eta,Vec3& refractDirection)
{
	double ndoti = Dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);
	if (k >= 0.0f) {
		refractDirection = eta * i - n * (eta * ndoti + sqrt(k));
		return true;
	}
	// total internal reflection. There is no refraction in this case
	else return false;
}
inline Vec3 Limit(const Vec3& src, double bottom, double top)
{
	Vec3 result;
	if (src.r < bottom) {
		result.r = bottom;
	} else if (src.r > top) {
		result.r = top;
	} else {
		result.r = src.r;
	}
	if (src.g < bottom) {
		result.g = bottom;
	} else if (src.g > top) {
		result.g = top;
	} else {
		result.g = src.g;
	}
	if (src.b < bottom) {
		result.b = bottom;
	} else if (src.b > top) {
		result.b = top;
	} else {
		result.b = src.b;
	}
	return result;
}

// ----------------------------------------------------------------------------------------------------------------------------

inline Mat4 BiasMatrix()
{
	Mat4 B;
	B[0] = 0.5f; B[4] = 0.0f; B[8] = 0.0f; B[12] = 0.5f;
	B[1] = 0.0f; B[5] = 0.5f; B[9] = 0.0f; B[13] = 0.5f;
	B[2] = 0.0f; B[6] = 0.0f; B[10] = 0.5f; B[14] = 0.5f;
	B[3] = 0.0f; B[7] = 0.0f; B[11] = 0.0f; B[15] = 1.0f;
	return B;
}
inline Mat4 BiasMatrixInverse()
{
	Mat4 BI;
	BI[0] = 2.0f; BI[4] = 0.0f; BI[8] = 0.0f; BI[12] = -1.0f;
	BI[1] = 0.0f; BI[5] = 2.0f; BI[9] = 0.0f; BI[13] = -1.0f;
	BI[2] = 0.0f; BI[6] = 0.0f; BI[10] = 2.0f; BI[14] = -1.0f;
	BI[3] = 0.0f; BI[7] = 0.0f; BI[11] = 0.0f; BI[15] = 1.0f;
	return BI;
}
inline Mat4 ViewMatrix(const Vec3 &x, const Vec3 &y, const Vec3 &z, const Vec3 &position)
{
	Mat4 V;
	V[0] = x.x; V[1] = y.x; V[2] = z.x; V[3] = 0.0f;
	V[4] = x.y; V[5] = y.y; V[6] = z.y; V[7] = 0.0f;
	V[8] = x.z; V[9] = y.z; V[10] = z.z; V[11] = 0.0f;
	V[12] = -Dot(x, position); V[13] = -Dot(y, position); V[14] = -Dot(z, position); V[15] = 1.0f;
	return V;
}
inline Mat4 ViewMatrixInverse(Mat4 &V)
{
	Mat4 VI;
	VI[0] = V[0]; VI[1] = V[4]; VI[2] = V[8]; VI[3] = 0.0f;
	VI[4] = V[1]; VI[5] = V[5]; VI[6] = V[9]; VI[7] = 0.0f;
	VI[8] = V[2]; VI[9] = V[6]; VI[10] = V[10]; VI[11] = 0.0f;

	VI[12] = -(VI[0] * V[12] + VI[4] * V[13] + VI[8] * V[14]);
	VI[13] = -(VI[1] * V[12] + VI[5] * V[13] + VI[9] * V[14]);
	VI[14] = -(VI[2] * V[12] + VI[6] * V[13] + VI[10] * V[14]);
	VI[15] = 1.0f;
	return VI;
}
inline Mat4 OrthoProjectionMatrix(double left, double right, double bottom, double top, double n, double f)
{
	Mat4 OP;
	OP[0] = 2.0f / (right - left);OP[1] = 0.0f;OP[2] = 0.0f;OP[3] = 0.0f;
	OP[4] = 0.0f;OP[5] = 2.0f / (top - bottom);OP[6] = 0.0f;OP[7] = 0.0f;
	OP[8] = 0.0f;OP[9] = 0.0f;OP[10] = -2.0f / (f - n);OP[11] = 0.0f;

	OP[12] = -(right + left) / (right - left);
	OP[13] = -(top + bottom) / (top - bottom);
	OP[14] = -(f + n) / (f - n);
	OP[15] = 1.0f;
	return OP;
}
inline Mat4 PerspectiveProjectionMatrix(double fovy, double x, double y, double n, double f)
{
	Mat4 PP;
	double coty = 1.0f / tan(fovy * (double)M_PI / 360.0f);
	double aspect = x / (y > 0.0f ? y : 1.0f);
	PP[0] = coty / aspect; PP[1] = 0.0f; PP[2] = 0.0f; PP[3] = 0.0f;
	PP[4] = 0.0f; PP[5] = coty; PP[6] = 0.0f; PP[7] = 0.0f;
	PP[8] = 0.0f; PP[9] = 0.0f; PP[10] = (n + f) / (n - f); PP[11] = -1.0f;
	PP[12] = 0.0f; PP[13] = 0.0f; PP[14] = 2.0f * n * f / (n - f); PP[15] = 0.0f;
	return PP;
}
inline Mat4 PerspectiveProjectionMatrixInverse(Mat4 &PP)
{
	Mat4 PPI;
	PPI[0] = 1.0f / PP[0]; PPI[1] = 0.0f; PPI[2] = 0.0f; PPI[3] = 0.0f;
	PPI[4] = 0.0f; PPI[5] = 1.0f / PP[5]; PPI[6] = 0.0f; PPI[7] = 0.0f;
	PPI[8] = 0.0f; PPI[9] = 0.0f; PPI[10] = 0.0f; PPI[11] = 1.0f / PP[14];
	PPI[12] = 0.0f; PPI[13] = 0.0f; PPI[14] = 1.0f / PP[11]; PPI[15] = -PP[10] / (PP[11] * PP[14]);
	return PPI;
}
inline Mat4 ScaleMatrix(double x, double y, double z)
{
	Mat4 S;
	S[0] = x; S[1] = 0.0f; S[2] = 0.0f; S[3] = 0.0f;
	S[4] = 0.0f; S[5] = y; S[6] = 0.0f; S[7] = 0.0f;
	S[8] = 0.0f; S[9] = 0.0f; S[10] = z; S[11] = 0.0f;
	S[12] = 0.0f; S[13] = 0.0f; S[14] = 0.0f; S[15] = 1.0f;
	return S;
}
inline Mat4 ScaleMatrixInverse(double x, double y, double z)
{
	Mat4 S;
	S[0] = 1/x; S[1] = 0.0f; S[2] = 0.0f; S[3] = 0.0f;
	S[4] = 0.0f; S[5] = 1/y; S[6] = 0.0f; S[7] = 0.0f;
	S[8] = 0.0f; S[9] = 0.0f; S[10] = 1/z; S[11] = 0.0f;
	S[12] = 0.0f; S[13] = 0.0f; S[14] = 0.0f; S[15] = 1.0f;
	return S;
}
inline Mat4 TranslationMatrix(double x, double y, double z)
{
	Mat4 T;
	T[0] = 1.0f; T[1] = 0.0f; T[2] = 0.0f; T[3] = 0.0f;
	T[4] = 0.0f; T[5] = 1.0f; T[6] = 0.0f; T[7] = 0.0f;
	T[8] = 0.0f; T[9] = 0.0f; T[10] = 1.0f; T[11] = 0.0f;
	T[12] = x; T[13] = y; T[14] = z; T[15] = 1.0f;
	return T;
}
inline Mat4 TranslationMatrixInverse(double x, double y, double z)
{
	Mat4 T;
	T[0] = 1.0f; T[1] = 0.0f; T[2] = 0.0f; T[3] = 0.0f;
	T[4] = 0.0f; T[5] = 1.0f; T[6] = 0.0f; T[7] = 0.0f;
	T[8] = 0.0f; T[9] = 0.0f; T[10] = 1.0f; T[11] = 0.0f;
	T[12] = -x; T[13] = -y; T[14] = -z; T[15] = 1.0f;
	return T;
}
inline Mat4 RotateMatrix(double angle, const Vec3 &u)
{
	Mat4 R;
	angle = angle / 180.0f * (double)M_PI;
	Vec3 v = Normalize(u);
	double c = 1.0f - cos(angle), s = sin(angle);

	R[0] = 1.0f + c * (v.x * v.x - 1.0f);
	R[1] = c * v.x * v.y + v.z * s;
	R[2] = c * v.x * v.z - v.y * s;
	R[3] = 0.0f;

	R[4] = c * v.x * v.y - v.z * s;
	R[5] = 1.0f + c * (v.y * v.y - 1.0f);
	R[6] = c * v.y * v.z + v.x * s;
	R[7] = 0.0f;

	R[8] = c * v.x * v.z + v.y * s;
	R[9] = c * v.y * v.z - v.x * s;
	R[10] = 1.0f + c * (v.z * v.z - 1.0f);
	R[11] = 0.0f;

	R[12] = 0.0f;
	R[13] = 0.0f;
	R[14] = 0.0f;
	R[15] = 1.0f;

	return R;
}
inline Mat4 RotateMatrixInverse(double angle, const Vec3 &u)
{
	Mat4 R;
	angle = angle / 180.0f * (double)M_PI;
	Vec3 v = Normalize(u);
	double c = 1.0f - cos(angle), s = sin(angle);

	R[0] = 1.0f + c * (v.x * v.x - 1.0f);
	R[1] = c * v.x * v.y - v.z * s;
	R[2] = c * v.x * v.z + v.y * s;
	R[3] = 0.0f;

	R[4] = c * v.x * v.y + v.z * s;
	R[5] = 1.0f + c * (v.y * v.y - 1.0f);
	R[6] = c * v.y * v.z - v.x * s;
	R[7] = 0.0f;

	R[8] = c * v.x * v.z - v.y * s;
	R[9] = c * v.y * v.z + v.x * s;
	R[10] = 1.0f + c * (v.z * v.z - 1.0f);
	R[11] = 0.0f;

	R[12] = 0.0f;
	R[13] = 0.0f;
	R[14] = 0.0f;
	R[15] = 1.0f;

	return R;
}
// ----------------------------------------------------------------------------------------------------------------------------

#endif