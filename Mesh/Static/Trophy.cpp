#include "Trophy.h"

#include "core/renderwindow.h"
#include "Physics/BoundingBox.h"

Trophy::Trophy(Shader* Shader,  glm::mat4 modelMat)
	:Mesh::Mesh(Shader, modelMat)
{

}

Trophy::Trophy(Shader* Shader,  glm::mat4 modelMat, const char* ObjFile, const char* texture)
	: Mesh::Mesh(Shader, modelMat, ObjFile, texture)
{

}


Trophy::~Trophy()
{
	Mesh::~Mesh();
}

bool Trophy::onBeginOverlap(CollisionPrimitive* primitive)
{
	if(Mesh::onBeginOverlap(primitive))
	{
		//todo send points
		destroy();
		return true;
	}
	return false;
}

void Trophy::init()
{
	/*mBoxComponent = new BoundingBox(getLocation(), glm::vec3(1.f, 1.f, 1.f), RenderWindow::Get()->getShader("color"));
	mBoxComponent->init();*/

	mCollisionPrimitive = new AABB(getLocation(), glm::vec3(1.f,1.f,1.f));


	Mesh::init();
}

void Trophy::draw()
{
		/*mBoxComponent->draw();*/

		/*mCollisionPrimitive->draw();*/

		Mesh::draw();
	
}
