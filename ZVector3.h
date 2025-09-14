#pragma once

#include <iostream>

class ZVector3 {
public:
	double x, y, z;

	ZVector3(double x = 0.0, double y = 0.0, double z = 0.0)
		: x(x), y(y), z(z) { }

	ZVector3(const ZVector3& other)
		:x(other.x), y(other.y), z(other.z) {}

	ZVector3 operator+(const ZVector3& other) const {
		return ZVector3(x + other.x, y + other.y, z + other.z);
	}

	ZVector3 operator-(const ZVector3& other) const {
		return ZVector3(x - other.x, y - other.y, z - other.z);
	}

	double dot(const ZVector3& other) const {
		return (x * other.x + y * other.y + z * other.z);
	}

	ZVector3 cross(const ZVector3& other) const {
		return ZVector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	friend std::ostream& operator <<(std::ostream& os, const ZVector3& vec) {
		os << "ZVector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
};