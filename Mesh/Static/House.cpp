#include "House.h"

#include "core/renderwindow.h"
#include "Physics/BoundingBox.h"


House::House(Shader* Shader,  glm::mat4 modelMat, const char* ObjFile, const char* texture,bool bDoor)
	:Mesh(Shader,modelMat,ObjFile,texture),bDrawDoor(bDoor)
{
}

House::~House()
{
}

void House::init()
{
	if(bDrawDoor)
	{
		glm::mat4 Doorloc = glm::translate(mModelMat, glm::vec3(10.f, 0.f, 0.f) * getScale());
        mDoor = new Door(mShaderProgram, Doorloc, "../3Dprog22/ObjFiles/Door.obj", "../3Dprog22/Textures/tree.jpg");
		//todo change location of it
		mDoor->init();
	}

	
	Mesh::init();
}

void House::draw()
{

	if(mDoor)
		mDoor->draw();

	Mesh::draw();
}

bool House::onBeginOverlap(CollisionPrimitive* primitive)
{
	if(mDoor)mDoor->onBeginOverlap(primitive);
	if (Mesh::onBeginOverlap(primitive))
	{
		//swap scene
		RenderWindow::Get()->changeScene(1);

		return true;
	}
	return false;
}

