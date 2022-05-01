#include "Vertex.h"
#include <fstream>

Vertex::Vertex()
{
}

Vertex::Vertex(float x, float y, float z)
	:x(x), y(y), z(z)
{

}

Vertex::Vertex(float x, float y, float z, float t1, float t2)
	: x(x), y(y), z(z), t1(t1), t2(t2)
{
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
	:x(x),y(y),z(z),r(r),g(g),b(b)
{
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float t1, float t2)
	: x(x), y(y), z(z), r(r), g(g), b(b), t1(t1), t2(t2)
{
}

Vertex::Vertex(glm::vec3 cord, glm::vec3 norm, glm::vec2 texture)
	: x(cord.x), y(cord.y), z(cord.z), normals(norm), t1(texture.x), t2(texture.y)
{
}

Vertex::~Vertex()
{

}


std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	os << std::fixed;
	os << v.x << " " << v.y << " " << v.z;
	os << " " << v.r << " " << v.g << " " << v.b;
	os << " " << v.t1 << " " << v.t2 << "\n";
	return os;
}