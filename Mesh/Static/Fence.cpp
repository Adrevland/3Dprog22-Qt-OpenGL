#include "Fence.h"

#include "core/renderwindow.h"

Fence::Fence(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader, modelMat, ObjFile, texture)
{
}

Fence::~Fence()
{
}

void Fence::init()
{
	mCollisionPrimitive = new AABB(getLocation(), getScale());
	Mesh::init();
}

void Fence::draw()
{
	Mesh::draw();
}

bool Fence::onBeginOverlap(CollisionPrimitive* primitive)
{
	if (Mesh::onBeginOverlap(primitive))
	{
		//todo send points
		//player
		if (primitive == RENDERWINDOW->getLevel()->getPlayer()->getcollision())
		{
			//todo push player
			RENDERWINDOW->getLevel()->getPlayer()->pushback(RENDERWINDOW->getLevel()->getPlayer()->getLocation());
			return true;
		}
		//npc
		RENDERWINDOW->getLevel()->getNpc()->handleCollision(primitive);
		if (primitive == RENDERWINDOW->getLevel()->getNpc()->getcollision())
		{
			
			RENDERWINDOW->getLevel()->getNpc()->pushback(RENDERWINDOW->getLevel()->getNpc()->getLocation());

			return true;
		}
	}
	return false;
}
