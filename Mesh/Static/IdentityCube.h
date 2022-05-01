#pragma once
#include "Mesh/Mesh.h"

class IdentityCube
	:public Mesh
{
public:
	IdentityCube(Shader* Shader, glm::mat4 modelMat);
	~IdentityCube();

	 void init() override;
	 void draw() override;
	 void initskybox();
	 void drawskybox(unsigned int Textureid);
private:
	const glm::vec3 color{ 1.f, 0.f, 1.f };
};

