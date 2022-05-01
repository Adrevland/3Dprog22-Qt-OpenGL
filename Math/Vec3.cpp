#include "Vec3.h"
#include <string>
#include "core/logger.h"
#include <cmath>

aMath::Vec3::Vec3(const double x, const double y, const double z)
	:x(x),y(y),z(z)
{
}

aMath::Vec3::~Vec3()
{
}

aMath::Vec3::Vec3(const Vec3& vec)
	:x(vec.x), y(vec.y), z(vec.z)
{
}

aMath::Vec3 aMath::Vec3::operator+(const Vec3& vec) const
{
	return Vec3{ vec.x + x, vec.y + y, vec.z + z };
}

aMath::Vec3& aMath::Vec3::operator+=(const Vec3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

aMath::Vec3 aMath::Vec3::operator-(const Vec3& vec) const
{
	return Vec3{ x - vec.x, y - vec.y, z - vec.z };
}

aMath::Vec3& aMath::Vec3::operator-=(const Vec3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

aMath::Vec3 aMath::Vec3::operator*(double value) const
{
	return Vec3{ x * value, y * value, z * value };
}

aMath::Vec3& aMath::Vec3::operator*=(double value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

aMath::Vec3 aMath::Vec3::operator/(double value) const
{
	return Vec3{ x / value, y / value, z / value };
}

aMath::Vec3& aMath::Vec3::operator/=(double value)
{
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

aMath::Vec3& aMath::Vec3::operator=(const Vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

bool aMath::Vec3::operator!=(const Vec3& vec) const
{
	return x != vec.x && y != vec.y && z != vec.z;
}

bool aMath::Vec3::operator==(const Vec3& vec) const
{
	return x == vec.x && y == vec.y && z == vec.z;
}

double aMath::Vec3::magnitude() const
{
	return sqrt(square());
}

double aMath::Vec3::square() const
{
	return x * x + y * y + z * z;
}

aMath::Vec3 aMath::Vec3::cross_product(const Vec3& vec) const
{
	return Vec3{ y * vec.z - z * vec.y,z * vec.x - x * vec.z,x * vec.y - y * vec.x };
}

double aMath::Vec3::distance(const Vec3& vec) const
{
	return (*this-vec).magnitude();
}

aMath::Vec3& aMath::Vec3::Normalize()
{
	*this /= magnitude();
	return *this;
}

double aMath::Vec3::dot_product(const Vec3& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

void aMath::Vec3::log(Logger *mLogger) const
{
	//x{1},y{1},z{1};
	mLogger->logText(std::string{ "x{" + std::to_string(x) + "},y{" + std::to_string(y) + "},z{" + std::to_string(z) + "}" }, LogType::HIGHLIGHT);

}
