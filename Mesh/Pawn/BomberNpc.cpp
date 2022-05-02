#include "Mesh/Pawn/BomberNpc.h"
#include "Core/renderwindow.h"
#include "glm/glm.hpp"
#include <chrono>

BomberNpc::BomberNpc(const char* obj, const char* texture, Mesh* Verticestofollow)
	:Mesh(RENDERWINDOW->getShader("lightshadow"), glm::mat4{ 1.f }, obj, texture)
{
	auto tmp = Verticestofollow->getVertices();
	for(auto& vert : tmp)
	{
		followpoints.emplace_back(vert.getCordinates());
	}
	vecsize = followpoints.size();
}

BomberNpc::BomberNpc(const char* obj, const char* texture, std::vector<glm::vec3> Verticestofollow)
	:Mesh(RENDERWINDOW->getShader("lightshadow"), glm::mat4{ 1.f }, obj, texture), followpoints(Verticestofollow)
{
	vecsize = Verticestofollow.size();
}

BomberNpc::~BomberNpc()
{
}

void BomberNpc::init()
{
	mModelMat = glm::scale(mModelMat, glm::vec3(10.f));
	lasttime = std::chrono::high_resolution_clock::now();
	Mesh::init();
}

void BomberNpc::draw()
{
	Mesh::draw();
}

void BomberNpc::tick()
{
	
	veclocation += speed;
	if (veclocation >= vecsize)
	{
		veclocation = vecsize-1;
		speed *= -1;
	}
	if (veclocation <= 0)
	{
		veclocation = 0;
		speed *= -1;
	}
	setLocation(followpoints.at(veclocation));

	//drop bomb;
	auto currenttime = std::chrono::high_resolution_clock::now();
	auto seconds = std::chrono::duration<float>(currenttime - lasttime).count();

	if(seconds >= 2.f)
	{
		dropbomb();
		lasttime = std::chrono::high_resolution_clock::now();
	}
}

void BomberNpc::dropbomb()
{
	LOG_WARNING("Dropping bomb XD!");
}
