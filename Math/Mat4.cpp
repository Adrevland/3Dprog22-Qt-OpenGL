#include "Mat4.h"

aMath::Mat4::Mat4()
{
	setToIdentity();
}

aMath::Mat4::Mat4(QMatrix4x4 qmat)
{

	//TODO mMat[0][0] = qmat(0,0) maybe later, to much work right now 

	const auto qdata = qmat.data();
	mMat[0][0] = qdata[0]; mMat[0][1] = qdata[1]; mMat[0][2] = qdata[2]; mMat[0][3] = qdata[3];
	mMat[1][0] = qdata[4]; mMat[1][1] = qdata[5]; mMat[1][2] = qdata[6]; mMat[1][3] = qdata[7];
	mMat[2][0] = qdata[8]; mMat[2][1] = qdata[9]; mMat[2][2] = qdata[10]; mMat[2][3] = qdata[11];
	mMat[3][0] = qdata[12]; mMat[3][1] = qdata[13]; mMat[3][2] = qdata[14]; mMat[3][3] = qdata[15];
}


aMath::Mat4::~Mat4()
{
}

void aMath::Mat4::setToIdentity()
{
	mMat[0][0] = 1; mMat[0][1] = 0; mMat[0][2] = 0; mMat[0][3] = 0;
	mMat[1][0] = 0; mMat[1][1] = 1; mMat[1][2] = 0; mMat[1][3] = 0;
	mMat[2][0] = 0; mMat[2][1] = 0; mMat[2][2] = 1; mMat[2][3] = 0;
	mMat[3][0] = 0; mMat[3][1] = 0; mMat[3][2] = 0; mMat[3][3] = 1;
}

void aMath::Mat4::translate(double x, double y, double z)
{
	mMat[0][3] += x;
	mMat[1][3] += y;
	mMat[2][3] += z;
}

void aMath::Mat4::translate(double x, double y)
{
	mMat[0][3] += x;
	mMat[1][3] += y;
}

void aMath::Mat4::translate(aMath::Vec3 vec)
{
	mMat[0][3] += vec.x;
	mMat[1][3] += vec.y;
	mMat[2][3] += vec.z;
}

void aMath::Mat4::setLocation(aMath::Vec3 location)
{
	mMat[0][3] = location.x;
	mMat[1][3] = location.y;
	mMat[2][3] = location.z;
}

void aMath::Mat4::rotate()
{

}

void aMath::Mat4::scale(aMath::Vec3 vec)
{
	mMat[0][0] = vec.x;
	mMat[1][1] = vec.y;
	mMat[2][2] = vec.z;
}

void aMath::Mat4::scale(double scale)
{
	mMat[0][0] *= scale;
	mMat[1][1] *= scale;
	mMat[2][2] *= scale;
}

void aMath::Mat4::lookAt(aMath::Vec3 eye, aMath::Vec3 center, aMath::Vec3 up)
{

}

aMath::Vec3 aMath::Mat4::getLocation() const
{
	return aMath::Vec3(mMat[0][3], mMat[1][3], mMat[2][3]);
}

