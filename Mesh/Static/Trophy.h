#pragma once
#include "Mesh/Mesh.h"
class BoundingBox;

class Trophy :
    public Mesh
{
public:

	Trophy(Shader* Shader, glm::mat4 modelMat);
	Trophy(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture);
	~Trophy();

	bool onBeginOverlap(CollisionPrimitive* primitive) override;
	void init() override;
	void draw() override;
private:
	//BoundingBox* mBoxComponent;
};

