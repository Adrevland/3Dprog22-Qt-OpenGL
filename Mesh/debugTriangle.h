#pragma once
#include "Mesh.h"
class DebugTriangle :
    public Mesh
{
public:
    DebugTriangle();
    ~DebugTriangle();

	void init()override;
    void draw()override;

    void updateverts(std::vector<Vertex> verts);

private:
protected:

};

