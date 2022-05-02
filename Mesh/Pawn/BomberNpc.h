#pragma once
#include "Mesh/Mesh.h"
#include <chrono>

class BomberNpc
	:public Mesh
{
public:
	BomberNpc(const char* obj, const char* texture, Mesh* Verticestofollow);
	BomberNpc(const char* obj, const char* texture, std::vector<glm::vec3> Verticestofollow);
	~BomberNpc();

	void init() override;
	void draw() override;

	void tick();
	void dropbomb();
private:
	float speed{ 2.f };

	std::chrono::steady_clock::time_point lasttime;

	int veclocation{ 0 };
	int vecsize{ 0 };
	std::vector<glm::vec3> followpoints;

};