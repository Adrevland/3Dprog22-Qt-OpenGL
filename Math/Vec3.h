#pragma once

#include "core/logger.h"

namespace aMath{
class Vec3
{
public:
    Vec3(){};
    Vec3(const double x,const double y,const double z);
	~Vec3();


	double x{ 0 }, y{ 0 }, z{ 0 };

    Vec3(const Vec3& vec);    //copy constructor
    Vec3 operator+(const Vec3& vec)const;    //addition
    Vec3& operator+=(const Vec3& vec);  ////assigning new result to the vector
    Vec3 operator-(const Vec3& vec)const;    //substraction
    Vec3& operator-=(const Vec3& vec);  //assigning new result to the vector
    Vec3 operator*(double value)const;    //multiplication
    Vec3& operator*=(double value);  //assigning new result to the vector.
    Vec3 operator/(double value)const;    //division
    Vec3& operator/=(double value);  //assigning new result to the vector
    Vec3& operator=(const Vec3& vec);

    bool operator!=(const Vec3&vec)const;
    bool operator==(const Vec3& vec)const;

    [[nodiscard]] double magnitude()const;
    [[nodiscard]] double square()const;
    [[nodiscard]] double distance(const Vec3&vec) const;
    Vec3& Normalize() ;
    [[nodiscard]] double dot_product(const Vec3& vec)const;
    [[nodiscard]] Vec3 cross_product(const Vec3& vec)const;

    void log(Logger *mlogger)const;
private:

};

}