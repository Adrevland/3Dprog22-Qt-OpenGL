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
    void sendTrophyspoints(glm::vec3 point) { TrophyPoints.emplace_back(point); }
protected:
    void setonmap();
    std::vector<glm::vec3> TrophyPoints;

    glm::vec3 frontVector{0.f,1.f,0.f};

    float speed{ 0.5f };
    Heightmap* mHeightmap{ nullptr };

    CollisionPrimitive* rightSensor{ nullptr };
    CollisionPrimitive* LeftSensor{ nullptr };
    CollisionPrimitive* ForwardSensor{ nullptr };
};

