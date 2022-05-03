#pragma once
#include "Mesh/Mesh.h"
class Heightmap;

class Npc :
    public Mesh
{
public:
    Npc(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture);
    ~Npc();

    void init() override;
    void draw() override;

    bool onBeginOverlap(CollisionPrimitive* primitive);

    void tick(float deltatime);

    void setHeightmap(Heightmap* map) { mHeightmap = map; }
    void sendTrophyspoints(std::vector<glm::vec3> trophyvecs) { TrophyPoints = trophyvecs; }
protected:

    std::vector<glm::vec3> TrophyPoints;

    float speed{ 0.5f };
    Heightmap* mHeightmap{ nullptr };

    CollisionPrimitive* rightSensor{ nullptr };
    CollisionPrimitive* LeftSensor{ nullptr };
    CollisionPrimitive* ForwardSensor{ nullptr };
};

