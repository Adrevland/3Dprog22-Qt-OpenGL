#pragma once
#include "Mesh/Mesh.h"
class Fence :
    public Mesh
{
public:
    Fence(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture);
    ~Fence();

    void init();
    void draw();


protected:
     bool onBeginOverlap(CollisionPrimitive* primitive)override;
};

