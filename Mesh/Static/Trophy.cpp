#include "Trophy.h"

#include "core/renderwindow.h"
#include "Physics/Collision.h"

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
		//player
		if(trophytype == TROPHYTYPE::Red && primitive == RENDERWINDOW->getLevel()->getPlayer()->getcollision())
		{
			RENDERWINDOW->getLevel()->PlayerScore += 1;
			LOG_HIGHLIGHT("PLAYER SCORE : " + std::to_string(RENDERWINDOW->getLevel()->PlayerScore));
			destroy();
			return true;
		}
		//npc
		if (trophytype == TROPHYTYPE::Blue && primitive == RENDERWINDOW->getLevel()->getNpc()->getcollision())
		{
			RENDERWINDOW->getLevel()->NpcScore += 1;
			auto tmp = primitive;
			auto tmp2 = RENDERWINDOW->getLevel()->getNpc()->getcollision();
			LOG_HIGHLIGHT("Npc SCORE : " + std::to_string(RENDERWINDOW->getLevel()->NpcScore));
			destroy();
			return true;
		}
	}
	return false;
}

void Trophy::init()
{
	/*mBoxComponent = new BoundingBox(getLocation(), glm::vec3(1.f, 1.f, 1.f), RenderWindow::Get()->getShader("color"));
	mBoxComponent->init();*/
	auto tmp = new AABB(getLocation(), glm::vec3(1.f, 1.f, 1.f));
	tmp->Extent = getScale();
	mCollisionPrimitive = tmp;
	
	Mesh::init();
}

void Trophy::draw()
{
		/*mBoxComponent->draw();*/

		/*mCollisionPrimitive->draw();*/

		Mesh::draw();
	
}

void Trophy::tick()
{
	//not in use yet. to make trophy bounce up and down
	glm::vec3 newlocation = getLocation();
	newlocation.z += sin(1);
	setLocation(newlocation);
	mCollisionPrimitive->Center = getLocation();
}
