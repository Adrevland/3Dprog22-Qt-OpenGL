#pragma once

#include "Mesh/Mesh.h"

class Door :
    public Mesh
{
public:

    Door(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture);
    ~Door();

    void init() override;
    void draw() override;

    bool onBeginOverlap(CollisionPrimitive* primitive) override;
protected:
    
private:
    //BoundingBox* mBoxComponent{ nullptr };
    float mDoorAngle{0.f};
};

