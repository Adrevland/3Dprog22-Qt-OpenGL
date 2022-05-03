#include "Bomb.h"
#include "Core/renderwindow.h"
#include "Core/Utils.h"
#include "Physics/Spawner.h"

Bomb::Bomb(const char* obj, const char* texture, Heightmap* hmap, glm::vec3 spawnlocation, Spawner* spawn)
	:Mesh(RENDERWINDOW->getShader("lightshadow"),glm::mat4{1.f},obj,texture),mHeightmap(hmap),owner(spawn)
{
	mModelMat = glm::translate(mModelMat, spawnlocation);
}

Bomb::~Bomb()
{
}

void Bomb::init()
{
	collision = new BSphere(getLocation(),Utils::radiusMesh(this));
	mCollisionPrimitive = collision;
	mModelMat = glm::scale(mModelMat, glm::vec3(5.f));
	Mesh::init();
}

void Bomb::draw()
{
	Mesh::draw();
	if(collision)collision->draw();
}

void Bomb::tick()
{
	if(!bexploded)setLocation(getLocation() - (mCamera->getWorldup()*0.3f));
	collision->Center = getLocation();
	if(getLocation().z <= mHeightmap->getHeight(getLocation()))
	{
		bexploded = true;
		explode();
	}
}

void Bomb::explode()
{
	collision->radius += 0.2f;
	//mModelMat = glm::scale(mModelMat, glm::vec3(2.f));
	if (collision->radius >= 50.f)
	{
		//LOG_HIGHLIGHT("Bomb deleted");
		destroy();
		//delete this;
		owner->destroy(this);
		delete collision;
	}
}

bool Bomb::onBeginOverlap(CollisionPrimitive* primitive)
{
	if (Mesh::onBeginOverlap(primitive))
	{
		//todo stun npc and player
		//stunning player either way
		if(RENDERWINDOW->getLevel()->getPlayer()->getcollision() == primitive)
		{
			RENDERWINDOW->getLevel()->getPlayer()->stun();
		}
		if (RENDERWINDOW->getLevel()->getNpc()->getcollision() == primitive)
		{
			LOG_HIGHLIGHT("Npc Stunned");
			RENDERWINDOW->getLevel()->getNpc()->stun();
		}
		destroy();
		return true;
	}
	return false;
}
