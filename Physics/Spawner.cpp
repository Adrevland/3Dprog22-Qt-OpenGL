#include "Spawner.h"

#include "Core/renderwindow.h"
#include "Mesh/Mesh.h"
#include "Mesh/Static/Bomb.h"

Spawner::Spawner(Mesh* owner,const char* obj, const char* texture, Heightmap* hmap)
	:mOwner(owner),mObj(obj), mTexture(texture),mHmap(hmap)
{
	lastspawn = std::chrono::high_resolution_clock::now();
}

Spawner::~Spawner()
{
}

void Spawner::CreateObjectToSpawn(const char* obj, const char* texture, Heightmap* hmap)
{
	auto bomb = new Bomb(obj, texture, hmap, mOwner->getLocation(),this);
	bomb->init();
	mSpawnedMeshes.emplace_back(bomb);
}

void Spawner::spawn(float spawnintervall)
{
	auto currenttime = std::chrono::high_resolution_clock::now();
	auto seconds = std::chrono::duration<float>(currenttime - lastspawn).count();
	if(seconds >= spawnintervall)
	{
		lastspawn = std::chrono::high_resolution_clock::now();
		CreateObjectToSpawn(mObj, mTexture, mHmap);
	}
	
}

void Spawner::destroy(Mesh* mesh)
{
	RENDERWINDOW->destroy(mesh);
	auto it = std::find(mSpawnedMeshes.begin(), mSpawnedMeshes.end(), mesh);
	if (it != mSpawnedMeshes.end())
	{
		mSpawnedMeshes.erase(it);
	}
}

void Spawner::draw()
{
	for(auto& mesh: mSpawnedMeshes)
	{
		mesh->draw();
	}
}

bool Spawner::checkoverlapp(Mesh* mesh)
{
	for(auto& smesh: mSpawnedMeshes)
	{
		smesh->checkOverlap(mesh);
	}
	return false;
}

void Spawner::simbomb()
{
	for (auto& mesh : mSpawnedMeshes)
	{
		dynamic_cast<Bomb*>(mesh)->tick();
	}
}
