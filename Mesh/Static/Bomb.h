#pragma once
#include "Mesh/Mesh.h"

class Spawner;
class Heightmap;

class Bomb :
    public Mesh
{
public:

    Bomb(const char* obj, const char* texture, Heightmap* hmap, glm::vec3 spawnlocation,Spawner* spawn);
    ~Bomb();

    void init() override;
    void draw() override;

    void tick();
    void explode();
	bool onBeginOverlap(CollisionPrimitive* primitive) override;

protected:
    Spawner* owner{ nullptr };

    bool bexploded{ false };
    BSphere* collision{ nullptr };
    Heightmap* mHeightmap{nullptr};
};

