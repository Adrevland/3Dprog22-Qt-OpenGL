#pragma once
#include "Mesh/Mesh.h"
#include "glm/glm.hpp"

class Heightmap;

enum LASTMOVE
{
    None = 0,
    ROTATELEFT,
    ROTATERIGHT,
    MOVEFORWARD,
};
struct Transform
{
    glm::vec3 location{1.f};
    glm::mat4 rotation{1.f};
    glm::vec3 scale{ 1.f };

    glm::mat4 calc()
    {
        glm::mat4 mat{ 1.f };
        glm::mat4 translation = glm::translate(mat, location);
        glm::mat4 scalemat = glm::scale( glm::mat4{1.f},scale);

        return translation*scalemat*rotation;
    }
};

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
    void sendTrophyspoints(std::vector<glm::vec3> trophyvecs) { TrophyPoints = trophyvecs; findTarget();}
    void sendTrophyspoints(glm::vec3 point) { TrophyPoints.emplace_back(point); findTarget();}

    void findTarget();
    void rotate(bool right);
    void moveForward(float amount);
    void moveright(float rightamount);
    glm::vec3 findDirection();
    void handleCollision(CollisionPrimitive* sensor);
    void updateSensors();
    void drawDebugLines(bool bdraw) override;

    void pushback(glm::vec3 pos);
protected:
    void setonmap();
    std::vector<glm::vec3> TrophyPoints;
    std::vector<glm::vec3> TakenTargets;

    glm::vec3 frontVector{0.f,1.f,0.f};

    float rotateAmount{ 30.f };

    Transform tform;

    LASTMOVE lastmove{LASTMOVE::None };

    CollisionPrimitive* lastcollision{nullptr};
    
    glm::vec3 target{ 100.f };
    int score{ 0 };

    float speed{ 0.1f };
    Heightmap* mHeightmap{ nullptr };

    CollisionPrimitive* rightSensor{ nullptr };
    CollisionPrimitive* LeftSensor{ nullptr };
    CollisionPrimitive* ForwardSensor{ nullptr };
    CollisionPrimitive* TopSensor{ nullptr };

    const float sensorDistance{ 10.f };

    std::chrono::steady_clock::time_point lasttime;
    std::chrono::steady_clock::time_point Collisiontime;
};

