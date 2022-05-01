#ifndef VERTEX_H
#define VERTEX_H
#pragma once

#include <ostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z, float t1, float t2);
	Vertex(float x, float y, float z, float r, float g, float b);
	Vertex(float x, float y, float z, float r, float g, float b, float t1, float t2);
	Vertex(glm::vec3 cord, glm::vec3 norm, glm::vec2 texture);
	~Vertex();

	void copyPosition(glm::vec3 pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
	};
	void copyNormals(glm::vec3 pos)
	{
		normals = pos;
		/*r = pos.r;
		g = pos.g;
		b = pos.b;*/
	};
	void copyUvs(glm::vec2 pos)
	{
		t1 = pos.x;
		t2 = pos.y;
	};
	glm::vec3 getCordinates() { return glm::vec3(x, y, z); }

	//latskap gidd ikke lag getters og setters
	float x{0.f}, y{ 0.f }, z{ 0.f };
	float r{ 1.f }, g{ 1.f }, b{ 1.f };
	float alpha{ 1.f };
	float t1{ 0.f }, t2{ 0.f };
	glm::vec3 normals;
	friend std::ostream& operator<< (std::ostream& os, const Vertex& v);

private:

	//Vector3d mXYZ;
	//Vector3d mNormal;
	
};

#endif
