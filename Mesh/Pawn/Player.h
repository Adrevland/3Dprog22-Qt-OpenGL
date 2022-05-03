#pragma once
#include "Mesh/static/Heightmap.h"
#include "Mesh/Mesh.h"
#include "Physics/BoundingBox.h"

class Heightmap;
class DebugTriangle;

class Player :
    public Mesh
{
public:
	Player(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture);
	~Player();

	void init() override;
	void draw() override;

	bool onBeginOverlap(CollisionPrimitive* primitive);

	void tick(float deltatime);
	void setSpeed(float s)
	{
		speed = s/50;
		if (s <= 0) speed = 0.1f;
	}
	void sendKeyboard(std::unordered_map<int,bool>&keyInput);
	void setHeightmap(Heightmap* map) { mHeightmap = map; }
	void pushback(glm::vec3 pos);
private:
	std::unordered_map<int, bool> mkeybInput;
	bool forwardillegal{ false };
	glm::vec3 lastVelocity{ 0.f };
	float forwardspeed{ 0 };
	float rightspeed{ 0 };

	float speed{ 0.5f };

	DebugTriangle* mBarytriangle{nullptr};
	Heightmap* mHeightmap{nullptr};

	bool isOnMap();
	void move();
	void rotate();
	std::chrono::steady_clock::time_point lasttime;
};

