#include "BoundingBox.h"
#include "Mesh/Static/IdentityCube.h"
#include "core/renderwindow.h"

BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(glm::vec3 center, glm::vec3 ext)
	:Center(center),Extent(ext)
{
	
}

BoundingBox::BoundingBox(glm::vec3 center, glm::vec3 ext, Shader* shader)
	: Center(center), Extent(ext), mShader(shader)
{

}


BoundingBox::~BoundingBox()
{
	//delete DebugLines;
}

void BoundingBox::draw()
{
	if(DebugLines)
	{
		DebugLines->setLocation(Center);
		DebugLines->draw();
	}
}

void BoundingBox::init()
{
	if (!mCamera) mCamera = RenderWindow::Get()->getCamera();
	if(mShader && mCamera) {
		DebugLines = new IdentityCube(mShader, glm::mat4{ 1.f });
		DebugLines->scale(Extent);
		
		DebugLines->init();
	}

}

bool BoundingBox::isOverlaping(glm::vec3 location)
{
	const auto maxX = Center.x + Extent.x;
	const auto maxY = Center.y + Extent.y;
	const auto maxZ = Center.z + Extent.z;

	const auto minX = Center.x - Extent.x;
	const auto minY = Center.y - Extent.y;
	const auto minZ = Center.z - Extent.z;

	if(location.x <= maxX && location.y <= maxY && location.z <= maxZ &&
		location.x >= minX && location.y >= minY && location.z >= minZ)
	{
		return true;
	}

	return false;
}

bool BoundingBox::isOverlaping(BoundingBox* box)
{
	auto distance = abs(Center - box->Center);
	auto maxDist = Extent + box->Extent;

	if(distance.x < maxDist.x && distance.y < maxDist.y && distance.z < maxDist.z)
	{
		return true;
	}
	return false;
}
