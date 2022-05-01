#include "Door.h"


#include "Core/renderwindow.h"
#include "Physics/BoundingBox.h"

Door::Door(Shader* Shader,  glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader,modelMat, ObjFile, texture)
{
}

Door::~Door()
{
}

void Door::init()
{
	//todo change location
	
	Mesh::init();

	
}

void Door::draw()
{

	Mesh::draw();
}

bool Door::onBeginOverlap(CollisionPrimitive* primitive)
{
	if (Mesh::onBeginOverlap(primitive))
	{
		if (mDoorAngle < 75)
		{
			mDoorAngle += 1.f;
			mModelMat = glm::rotate(mModelMat, glm::radians(1.f), glm::vec3(0.f, 0.f, 1.f));
		}
		/*if(mModelMat[3][2] < 20.f)
			mModelMat = glm::translate(mModelMat, glm::vec3(0.f,0.f,0.05f));*/
		return true;
	}
	return false;
}