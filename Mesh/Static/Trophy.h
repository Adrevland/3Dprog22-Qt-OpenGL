#pragma once
#include "Mesh/Mesh.h"
class BoundingBox;

enum TROPHYTYPE
{
	NONE = 0,
	Red = 1,
	Blue = 2,
};

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
	void tick();
	TROPHYTYPE trophytype{TROPHYTYPE::NONE};
private:
	//BoundingBox* mBoxComponent;
};

