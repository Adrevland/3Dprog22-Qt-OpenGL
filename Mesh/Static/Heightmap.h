#pragma once
#include "Mesh/Mesh.h"

class Heightmap :
    public Mesh
{
public:
    Heightmap(const float size,const char* map);
    Heightmap(const float size, const char* map, const char* text);
    ~Heightmap();

	void init() override;
	void draw() override;
    float getHeight(glm::vec3 pos);

    int mWidth{ 0 };
    int mHeight{ 0 };
private:
    
    float mSize;
    const char* mMapstr;

    void readmap();

   /* glm::vec3 barycentricCoordinates();*/
protected:

};

