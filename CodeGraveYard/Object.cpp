#include "Object.h"

Object::Object()
{
}

Object::Object(Vertex V)
{
	insertVert(V);
}

Object::~Object()
{
}

void Object::insertVert(Vertex V)
{
	mVertices.push_back(V);
}

void Object::insertIndice(std::vector<int> i)
{
	mIndices.push_back(i);
}
