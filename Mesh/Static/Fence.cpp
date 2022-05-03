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
		if (RENDERWINDOW->getLevel()->getNpc()->getcollision())
		{
			//todo push npc

			return true;
		}
	}
	return false;
}
