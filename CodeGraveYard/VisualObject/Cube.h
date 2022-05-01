#pragma once
#include "VisualObject.h"
#include "Math//Vertex.h"
#include "core/Texture.h"

class Cube : public VisualObject
{
public:
    Cube(Shader* shader);
	~Cube();
    void init(Camera& camera) override;
    void draw() override;

private:
    //QOpenGLContext* mContext;
    Texture* texture;
    
};

