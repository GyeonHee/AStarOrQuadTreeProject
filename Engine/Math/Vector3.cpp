#include "Vector3.h"

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3& other)
	: x(other.x), y(other.y), z(other.z)
{
}

Vector3& Vector3::operator=(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const
{
	if (scalar != 0.0f)
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	// 0 ³ª´©±â ¹æÁö.
	return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
	if (scalar != 0.0f)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}
	// 0 ³ª´©±â ¹æÁö.
	else
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}