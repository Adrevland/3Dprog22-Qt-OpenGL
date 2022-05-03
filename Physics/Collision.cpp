#include "Collision.h"
#include "Core/renderwindow.h"
#include "glm/glm.hpp"

Collision::Collision()
{
}

Collision::~Collision()
{
}

//bool Collision::isOverlapping(CollisionPrimitive* primitive1, CollisionPrimitive* primitive2)
//{
//	return primitive1->isOverlapping(primitive2);
//}


bool Collision::sat()
{
	return false;
}

bool Collision::isOverlapping(AABB* aabb, AABB* aabb1)
{
	auto distance = abs(aabb->Center - aabb1->Center);
	auto maxDist = aabb->Extent + aabb1->Extent;

	return distance.x < maxDist.x&& distance.y < maxDist.y&& distance.z < maxDist.z;
}


bool Collision::isOverlapping(CollisionPrimitive* primitive, OBB* obb)
{
	switch (primitive->shape)
	{
	case Shape::aabb:
		return Collision::isOverlapping(obb, reinterpret_cast<AABB*>(primitive));
		break;

	case Shape::obb:
		return Collision::isOverlapping(obb, reinterpret_cast<OBB*>(primitive));
		break;

	case Shape::bsphere:
		return Collision::isOverlapping(reinterpret_cast<BSphere*>(primitive),obb);
		break;
	default:
		LOG_REALERROR("Invalid Primitive shape");
		break;
	}
	return false;
}

bool Collision::isOverlapping(CollisionPrimitive* primitive, BSphere* sphere)
{
	switch (primitive->shape)
	{
	case Shape::aabb:
		return Collision::isOverlapping(sphere, reinterpret_cast<AABB*>(primitive));
		break;

	case Shape::obb:
		return Collision::isOverlapping(sphere, reinterpret_cast<OBB*>(primitive));
		break;

	case Shape::bsphere:
		return Collision::isOverlapping(sphere, reinterpret_cast<BSphere*>(primitive));
		break;
	default:
		LOG_REALERROR("Invalid Primitive shape");
		break;
	}
	return false;
}

bool Collision::isOverlapping(CollisionPrimitive* primitive, AABB* aabb)
{
	switch (primitive->shape)
	{
	case Shape::aabb:
		return Collision::isOverlapping(aabb, reinterpret_cast<AABB*>(primitive));
		break;

	case Shape::obb:
		return Collision::isOverlapping(reinterpret_cast<OBB*>(primitive), aabb);
		break;

	case Shape::bsphere:
		return Collision::isOverlapping(reinterpret_cast<BSphere*>(primitive),aabb);
		break;
	default:
		LOG_REALERROR("Invalid Primitive shape");
		break;
	}
	return false;
}

bool Collision::isOverlapping(OBB* obb, OBB* obb1)
{
	return false;
}

bool Collision::isOverlapping(OBB* obb, AABB* aabb)
{
	return false;
}

bool Collision::isOverlapping(BSphere* sphere, BSphere* sphere1)
{
	if (sphere == sphere1) return false;
	const auto maxDistance = sphere->radius + sphere1->radius;
	const auto distance = abs(sphere->Center - sphere1->Center);
	float len;
	if (distance == glm::vec3(0.f))
	{
		len = 0.f;
	}
	else  len = glm::length(distance);
	return  len < maxDistance;
	
}

bool Collision::isOverlapping(BSphere* sphere, OBB* obb)
{
	return false;
}

bool Collision::isOverlapping(BSphere* sphere, AABB* aabb)
{

	//yeeet
	glm::vec3 boxPoint;
	boxPoint.x = std::clamp<float>(sphere->Center.x, aabb->Center.x - aabb->Extent.x, aabb->Center.x + aabb->Extent.x);
	boxPoint.y = std::clamp<float>(sphere->Center.y, aabb->Center.y - aabb->Extent.y, aabb->Center.y + aabb->Extent.y);
	boxPoint.z = std::clamp<float>(sphere->Center.z, aabb->Center.z - aabb->Extent.z, aabb->Center.z + aabb->Extent.z);
	float dist = glm::distance(boxPoint, sphere->Center);

	return dist < sphere->radius;


}

CollisionPrimitive::CollisionPrimitive(glm::vec3 center)
	:Center(center)
{
}

CollisionPrimitive::~CollisionPrimitive()
{
}

bool CollisionPrimitive::isOverlapping(CollisionPrimitive* primitive)
{
	switch (shape)
	{
	case Shape::aabb:
		return Collision::isOverlapping(primitive, reinterpret_cast<AABB*>(this));
		break;

	case Shape::obb:
		return Collision::isOverlapping(primitive, reinterpret_cast<OBB*>(this));
		break;

	case Shape::bsphere:
		return Collision::isOverlapping(primitive, reinterpret_cast<BSphere*>(this));
		break;
	default:
		LOG_REALERROR("Invalid Primitive shape");
		break;
	}
	return false;
}

void CollisionPrimitive::draw()
{
	glm::mat4 mat{1.f};
	mat = glm::translate(mat, Center);
	OBB* obb{ nullptr };
	switch (shape)
	{
	case Shape::aabb:
		mat = glm::scale(mat, reinterpret_cast<AABB*>(this)->Extent);
		RenderWindow::Get()->drawDebugShape("cube", mat);
		break;
	case Shape::obb:
		 obb = static_cast<OBB*>(this);
		RenderWindow::Get()->drawDebugShape("cube", obb->orientation);
		break;
	case Shape::bsphere:
		mat = glm::scale(mat, glm::vec3(reinterpret_cast<BSphere*>(this)->radius));
		RenderWindow::Get()->drawDebugShape("sphere", mat);
		break;
	}
}

AABB::AABB(glm::vec3 center, glm::vec3 ext)
	:CollisionPrimitive(center), Extent(ext)
{
	shape = Shape::aabb;
}

AABB::~AABB()
{
}

OBB::OBB(glm::vec3 center, glm::vec3 ext, glm::mat4 orientation)
	:CollisionPrimitive(center),Extent(ext),orientation(orientation)
{
	shape = Shape::obb;
}

OBB::~OBB()
{
}

BSphere::BSphere(glm::vec3 center, float radius)
	:CollisionPrimitive(center),radius(radius)
{
	shape = Shape::bsphere;
}

BSphere::~BSphere()
{
}
