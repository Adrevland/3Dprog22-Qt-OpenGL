#include "Bomb.h"
#include "Core/renderwindow.h"
#include "Core/Utils.h"

Bomb::Bomb(const char* obj, const char* texture, Heightmap* hmap, glm::vec3 spawnlocation)
	:Mesh(RENDERWINDOW->getShader("shadow"),glm::mat4{1.f},obj,texture),mHeightmap(hmap)
{
	mModelMat = glm::translate(mModelMat, spawnlocation);
}

Bomb::~Bomb()
{
}

void Bomb::init()
{
	collision = new BSphere(getLocation(),Utils::radiusMesh(this));
	
	Mesh::init();
}

void Bomb::draw()
{
	Mesh::draw();
}

void Bomb::tick()
{
	setLocation(getLocation() - mCamera->getWorldup());
	collision->Center = getLocation();
	if(getLocation().z >= mHeightmap->getHeight(getLocation()))
	{
		explode();
	}
}

void Bomb::explode()
{
	collision->radius += 1.f;
	mModelMat = glm::scale(mModelMat, glm::vec3(2.f));
	if (collision->radius >= 10.f)
	{
		LOG_HIGHLIGHT("Bomb deleted");
		destroy();
	}
}

bool Bomb::onBeginOverlap(CollisionPrimitive* primitive)
{
	if (Mesh::onBeginOverlap(primitive))
	{
		//todo stun npc and player

		destroy();
		return true;
	}
	return false;
}
