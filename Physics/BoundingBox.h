#pragma once
#include <glm/vec3.hpp>
#include "Mesh/Mesh.h"


class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(glm::vec3 center, glm::vec3 ext);
	BoundingBox(glm::vec3 center, glm::vec3 ext, Shader* shader);
	~BoundingBox();

	void draw();
	void init();
	bool isOverlaping(glm::vec3 location);
	bool isOverlaping(BoundingBox* box);
	glm::vec3 Center;
	glm::vec3 Extent;

	Mesh* DebugLines{ nullptr };
private:
	Shader* mShader{ nullptr };
	Camera* mCamera{ nullptr };
};

