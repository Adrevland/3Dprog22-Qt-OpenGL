#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


class AABB;
class OBB;
class BSphere;
class CollisionPrimitive;

enum Shape
{
	none = 0,
	aabb,
	obb,
	bsphere,
};

class Collision
{
public:
	Collision();
	~Collision();

	//static bool isOverlapping(CollisionPrimitive* primitive1, CollisionPrimitive* primitive2);

	static bool isOverlapping(CollisionPrimitive* primitive, AABB* aabb);
	static bool isOverlapping(CollisionPrimitive* primitive, OBB* obb);
	static bool isOverlapping(CollisionPrimitive* primitive, BSphere* sphere);

	static bool isOverlapping(BSphere* sphere, BSphere* sphere1);
	static bool isOverlapping(BSphere* sphere, OBB* obb);
	static bool isOverlapping(BSphere* sphere, AABB* aabb);

	static bool isOverlapping(OBB* obb, OBB* obb1);
	static bool isOverlapping(OBB* obb, AABB* aabb);

	static bool isOverlapping(AABB* aabb, AABB* aabb1);
	

private:

	bool sat();

};

class CollisionPrimitive
{
public:
	CollisionPrimitive(glm::vec3 center);
	~CollisionPrimitive();

	bool isOverlapping(CollisionPrimitive* primitive);

	void draw();
	glm::vec3 Center;

	Shape shape { Shape::none};
};

class AABB : public CollisionPrimitive
{
public:
	AABB(glm::vec3 center, glm::vec3 ext);
	~AABB();

	/*glm::vec3 Center;*/
	glm::vec3 Extent;

	bool isOverlapping(AABB* aabb) { return Collision::isOverlapping(this, aabb); }
	//bool isOverlapping(OBB* obb) { Collision::isOverlapping(this, obb); }
	bool isOverlapping(BSphere* sphere) { return Collision::isOverlapping(sphere, this); }
private:

};

class OBB : public CollisionPrimitive
{
public:
	OBB(glm::vec3 center, glm::vec3 ext, glm::mat4 orientation);
	~OBB();

	/*glm::vec3 Center;*/
	glm::vec3 Extent;
	glm::mat4 orientation; //enough for everything

	bool isOverlapping(AABB* aabb) { return Collision::isOverlapping(this, aabb); }
	bool isOverlapping(OBB* obb) { Collision::isOverlapping(this, obb); }
	bool isOverlapping(BSphere* sphere) { return Collision::isOverlapping(sphere, this); }

};

class BSphere : public CollisionPrimitive
{
public:
	BSphere(glm::vec3 center, float radius);
	~BSphere();

	/*glm::vec3 center;*/
	float radius;


	bool isOverlapping(AABB* aabb) { return Collision::isOverlapping(this, aabb); }
	bool isOverlapping(OBB* obb) { Collision::isOverlapping(this, obb); }
	bool isOverlapping(BSphere* sphere) { return Collision::isOverlapping(sphere, this); }
};