#include "Utils.h"
#include "glm/glm.hpp"
#include "Mesh/mesh.h"



glm::vec3 Utils::Barycentric(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{

	glm::vec2 ab = b - a;
	glm::vec2 ac = c - a;
	glm::vec3 n = glm::cross(glm::vec3(ab.x,ab.y,0), glm::vec3(ac.x,ac.y,0));
	float abcLen = glm::length(n);
	glm::vec3 baryc{1.f}; // til retur. Husk
	// u
	glm::vec2 p = b - point;
	glm::vec2 q = c - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
	baryc.x = n.z / abcLen;

	// v
	p = c - point;
	q = a - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
	baryc.y = n.z / abcLen;

	// w
	p = a - point;
	q = b - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
	baryc.z = n.z / abcLen;
	return baryc;

}

bool Utils::isInsideTri(glm::vec3 v)
{
	for (size_t i = 0; i < 3; i++)
	{
		if (v[i] < 0.f || v[i] > 1.f)
			return false;
	}
	return true;
}

std::string Utils::vecToString(glm::vec3 v)
{
	std::string s = "x: " + std::to_string(v.x) + " ";
	s += "y: " + std::to_string(v.y) + " ";
	s += "z: " + std::to_string(v.z);

	return s;
}

float Utils::radiusMesh(Mesh* mesh)
{
	float radius{0};
	auto location = mesh->getLocation();

	for(auto& vert: mesh->getVertices())
	{
		glm::vec3 vertloc = vert.getCordinates();

		auto dif = (vertloc - location).length();
		if (abs(dif) > radius) radius = abs(dif);
	}

	return (radius/2.f);
}
