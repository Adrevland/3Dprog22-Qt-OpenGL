#pragma once
#include "Door.h"
#include "Mesh/Mesh.h"


class House :
    public Mesh
{
public:
    House(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture, bool bDoor);
    ~House();

    void init() override;
    void draw() override;

protected:
    bool onBeginOverlap(CollisionPrimitive* primitive) override;
private:
    //BoundingBox* mBoxComponent{nullptr};
    Door* mDoor{nullptr};
    bool bDrawDoor{ true };
};

