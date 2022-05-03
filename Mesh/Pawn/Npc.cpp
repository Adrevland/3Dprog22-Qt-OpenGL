#include "Npc.h"

Npc::Npc(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader, modelMat, ObjFile, texture)
{
}

Npc::~Npc()
{
}

void Npc::init()
{
	Mesh::init();
}

void Npc::draw()
{
	Mesh::draw();
}

bool Npc::onBeginOverlap(CollisionPrimitive* primitive)
{
	return Mesh::onBeginOverlap(primitive);
}

void Npc::tick(float deltatime)
{
}
