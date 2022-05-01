#pragma once
#include <QMatrix4x4>
#include "Vec3.h"

namespace aMath{
class Mat4
{
public:
	Mat4(); // set to identity
	Mat4(QMatrix4x4 qmat); // copy a qmatrix
	~Mat4();
	void setToIdentity();
	void translate(double x, double y, double z);
	void translate(double x, double y);
	void translate(aMath::Vec3 vec);
	void setLocation(aMath::Vec3 location);
	void rotate();
	void scale(aMath::Vec3 vec);
	void scale(double scale);
	void lookAt(aMath::Vec3 eye, aMath::Vec3 center, aMath::Vec3 up);

	[[nodiscard]] aMath::Vec3 getLocation() const;

private:

	double mMat[4][4]{0};
};
}