#pragma once
#include "Mesh/Mesh.h"

class Heightmap;

class Bomb :
    public Mesh
{
public:

    Bomb(const char* obj, const char* texture, Heightmap* hmap, glm::vec3 spawnlocation);
    ~Bomb();

    void init() override;
    void draw() override;

    void tick();
    void explode();
    virtual bool onBeginOverlap(CollisionPrimitive* primitive);

protected:

    BSphere* collision{ nullptr };
    Heightmap* mHeightmap{nullptr};
};

