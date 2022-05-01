#pragma once
#include <vector>
#include "Math/Vertex.h"

class Object
{
public:
	Object();
	Object(Vertex V);
	
	~Object();

	void insertVert(Vertex V);
	void insertIndice(std::vector<int> i);
	std::vector<Vertex> GetVertices() { return mVertices; };
	std::vector<std::vector<int>> GetIndices() { return mIndices; };
private:
	std::vector<Vertex> mVertices;
	std::vector<std::vector<int>> mIndices;
};

