#pragma once
#include <limits>
#include <algorithm>
#include <cmath>
#include <d2d1.h>

extern float bits_to_float(std::uint32_t i);
#define M_PI 3.14159265358979323846
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

#define FLOAT32_NAN_BITS     ( std::uint32_t ) 0x7FC00000	// not a number!
#define FLOAT32_NAN          bits_to_float( FLOAT32_NAN_BITS )
#define VEC_T_NAN FLOAT32_NAN

#define ASSERT( _exp ) ( (void ) 0 )

template <typename T>
T clip_number(const T& n, const T& lower, const T& upper) {
	if (n < lower) return lower;
	if (n > upper) return upper;
	return n;
}

class Vector3 {
public:
	Vector3();
	Vector3(float, float, float);
	~Vector3();

	float x, y, z;

	Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	Vector3& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	bool operator==(const Vector3& v) {
		return (x == v.x) && (y == v.y);
	}
	Vector3 operator+(const Vector3& v) {
		return Vector3{ x + v.x, y + v.y, z + v.z };
	}
	Vector3 operator-(const Vector3& v) {
		return Vector3{ x - v.x, y - v.y, z - v.z };
	}
	Vector3 operator*(float fl) const {
		return Vector3(x * fl, y * fl, z * fl);
	}
	Vector3 operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	auto operator-(const Vector3& other) const -> Vector3 {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/(float other) const {
		Vector3 vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length2D() const
	{
		return sqrt((x * x) + (y * y));
	}
	void crossproduct(Vector3 v1, Vector3 v2, Vector3 cross_p) const //ijk = xyz
	{
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}
	Vector3 Cross(const Vector3& vOther) const
	{
		Vector3 res;
		crossproduct(*this, vOther, res);
		return res;
	}

	void init(float ix, float iy, float iz);
	Vector3 clamp();
	Vector3 clamped();
	Vector3 normalized();
	float normalize_float();
	float distance_to(const Vector3& other);
	void normalize();
	float length();
	float length_2d_sqr(void) const;
	float dot(const Vector3 other);
	float dot(const float* other);
};

// has to be here
inline Vector3 operator*(float lhs, const Vector3& rhs) {
	return Vector3(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}

struct matrix_t
{
	matrix_t() { }
	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		mat_val[0][0] = m00;	mat_val[0][1] = m01; mat_val[0][2] = m02; mat_val[0][3] = m03;
		mat_val[1][0] = m10;	mat_val[1][1] = m11; mat_val[1][2] = m12; mat_val[1][3] = m13;
		mat_val[2][0] = m20;	mat_val[2][1] = m21; mat_val[2][2] = m22; mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& vecOrigin)
	{
		mat_val[0][0] = xAxis.x; mat_val[0][1] = yAxis.x; mat_val[0][2] = zAxis.x; mat_val[0][3] = vecOrigin.x;
		mat_val[1][0] = xAxis.y; mat_val[1][1] = yAxis.y; mat_val[1][2] = zAxis.y; mat_val[1][3] = vecOrigin.y;
		mat_val[2][0] = xAxis.z; mat_val[2][1] = yAxis.z; mat_val[2][2] = zAxis.z; mat_val[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix_t(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& vecOrigin)
	{
		init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void set_origin(Vector3 const& p)
	{
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}

	inline void invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mat_val[i][j] = VEC_T_NAN;
			}
		}
	}

	float* operator[](int i) { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	const float* operator[](int i) const { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	float* base() { return &mat_val[0][0]; }
	const float* base() const { return &mat_val[0][0]; }

	float mat_val[3][4];
};

class Vector2 {
public:
	float x, y;

	Vector2() {
		x = 0; y = 0;
	};
	Vector2(float X, float Y) {
		x = X; y = Y;
	};
	Vector2(Vector3 vec) {
		x = vec.x; y = vec.y;
	}

	inline Vector2 operator*(const float n) const {
		return Vector2(x * n, y * n);
	}
	inline Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}
	inline Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}
	inline void operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const Vector2& v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const Vector2& v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return sqrt((x * x) + (y * y));
	}

	D2D1_RECT_F rect();
};
