#include "vectors.h"
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <random>

float bits_to_float(std::uint32_t i) {
	union convertor_t
	{
		float f;
		unsigned long ul;
	}tmp;

	tmp.ul = i;
	return tmp.f;
}

Vector3::Vector3(void) {
	x = y = z = 0.0f;
}

Vector3::Vector3(float fx, float fy, float fz) {
	x = fx;
	y = fy;
	z = fz;
}

Vector3::~Vector3(void) {
};

void Vector3::init(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}

Vector3 Vector3::clamp(void) {
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
	return Vector3(x, y, z);
}

Vector3 Vector3::clamped() {
	Vector3 clamped = *this;
	clamped.clamp();
	return clamped;
}

float Vector3::normalize_float() {
	Vector3 res = *this;
	float l = res.length();
	if (l != 0.0f) {
		res /= l;
	}
	else {
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}

float Vector3::distance_to(const Vector3& other) {
	Vector3 delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

void Vector3::normalize(void) {
	auto vec_normalize = [&](Vector3& v) {
		auto l = v.length();

		if (l != 0.0f) {
			v.x /= l;
			v.y /= l;
			v.z /= l;
		}
		else {
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	};

	vec_normalize(*this);
}

Vector3 Vector3::normalized(void) {
	Vector3 vec(*this);
	vec.normalize();

	return vec;
}

float Vector3::length(void) {
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
	};

	return sqrt(sqr(x) + sqr(y) + sqr(z));
}

float Vector3::length_2d_sqr(void) const {
	return (x * x + y * y);
}

float Vector3::dot(const Vector3 other) {
	return (x * other.x + y * other.y + z * other.z);
}

float Vector3::dot(const float* other) {
	const Vector3& a = *this;

	return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}

D2D1_RECT_F Vector2::rect() {
	return D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(INT_MAX), static_cast<FLOAT>(INT_MAX));
}
