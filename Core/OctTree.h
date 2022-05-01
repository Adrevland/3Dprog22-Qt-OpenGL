#pragma once

#include <glm/vec3.hpp>
#include "Physics/BoundingBox.h"

#include "Mesh/Mesh.h"

class OctTree
{
public:
	OctTree();
	OctTree(BoundingBox Bbox);
	~OctTree();

	bool remove(Mesh* mesh);
	OctTree* insert(Mesh* mesh);
	OctTree* find(const glm::vec3 &location);
	OctTree* find(Mesh* mesh);

	void move(Mesh* mesh);
	void drawTree();
	void divide();

	void checkCollision(Mesh* mesh);

private:
	//functions
	bool isLeaf() const;
	int sumGameObject();
	void collapse();
	//vector of meshes 
	std::vector<Mesh*> mMeshes;

	//center of quad and extent
	BoundingBox mBbox;

	std::vector<OctTree*> mChilds;

	const int MaxObject{ 3 }; // dont like to be smaller than 3
	const float MinExtentSize{1.f};  // overrides maxobject count dont want tiny octs

	bool bDrawDebugLines{ true };
};
	
