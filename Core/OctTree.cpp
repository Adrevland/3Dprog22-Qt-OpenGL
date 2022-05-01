#include "OctTree.h"

#include "renderwindow.h"

#define KILL delete
#define PEWPEW auto
#define ALWAYS for
#define FROM_MY_CHILD_LIST mChilds

OctTree::OctTree()
{
}


OctTree::OctTree(BoundingBox Bbox)
	:mBbox(Bbox)
{
	mBbox.init();
}

OctTree::~OctTree()
{
	ALWAYS (PEWPEW child: FROM_MY_CHILD_LIST)
		KILL child; // bye bye children
}

void OctTree::divide()
{

	BoundingBox a = mBbox;
	a.Extent /= 2.f;
	a.Center.y -= mBbox.Extent.y / 2.f;
	a.Center.x -= mBbox.Extent.x / 2.f;
	a.Center.z -= mBbox.Extent.z / 2.f;

	BoundingBox b = mBbox;
	b.Extent /= 2.f;
	b.Center.y -= mBbox.Extent.y / 2.f;
	b.Center.x += mBbox.Extent.x / 2.f;
	b.Center.z -= mBbox.Extent.z / 2.f;

	BoundingBox c = mBbox;
	c.Extent /= 2.f;
	c.Center.y += mBbox.Extent.y / 2.f;
	c.Center.x += mBbox.Extent.x / 2.f;
	c.Center.z -= mBbox.Extent.z / 2.f;

	BoundingBox d = mBbox;
	d.Extent /= 2.f;
	d.Center.y += mBbox.Extent.y / 2.f;
	d.Center.x -= mBbox.Extent.x / 2.f;
	d.Center.z -= mBbox.Extent.z / 2.f;

	BoundingBox e = mBbox;
	e.Extent /= 2.f;
	e.Center.y -= mBbox.Extent.y / 2.f;
	e.Center.x -= mBbox.Extent.x / 2.f;
	e.Center.z += mBbox.Extent.z / 2.f;

	BoundingBox f = mBbox;
	f.Extent /= 2.f;
	f.Center.y -= mBbox.Extent.y / 2.f;
	f.Center.x += mBbox.Extent.x / 2.f;
	f.Center.z += mBbox.Extent.z / 2.f;

	BoundingBox g = mBbox;
	g.Extent /= 2.f;
	g.Center.y += mBbox.Extent.y / 2.f;
	g.Center.x += mBbox.Extent.x / 2.f;
	g.Center.z += mBbox.Extent.z / 2.f;

	BoundingBox h = mBbox;
	h.Extent /= 2.f;
	h.Center.y += mBbox.Extent.y / 2.f;
	h.Center.x -= mBbox.Extent.x / 2.f;
	h.Center.z += mBbox.Extent.z / 2.f;

	mChilds.emplace_back(new OctTree(a));
	mChilds.emplace_back(new OctTree(b));
	mChilds.emplace_back(new OctTree(c));
	mChilds.emplace_back(new OctTree(d));
	mChilds.emplace_back(new OctTree(e));
	mChilds.emplace_back(new OctTree(f));
	mChilds.emplace_back(new OctTree(g));
	mChilds.emplace_back(new OctTree(h));

	if (bDrawDebugLines)
	{
		//RenderWindow::Get()->clearDebugBoxes();
		//drawTree();
		
	}
	
}

void OctTree::checkCollision(Mesh* mesh)
{
	auto tmp = find(mesh->getLocation());
	if(tmp)
		for(auto m: tmp->mMeshes)
		{
			m->checkOverlap(mesh);
		}
}

void OctTree::collapse()
{
	//test if i can collapse
	for(auto oct:mChilds)
	{
		
		if (sumGameObject() + 1 < MaxObject)
		{

			std::vector<Mesh*> mCopy;
			for (auto child : mChilds)
			{
				for (auto mesh : child->mMeshes)
				{
					mCopy.emplace_back(mesh);
				}

				delete child;
			}
			mChilds.clear();

			for (auto mesh : mCopy)
			{
				insert(mesh);
			}


		}
		oct->collapse();
	}
	
}


bool OctTree::remove(Mesh* mesh)
{

	const auto tmp = find(mesh->getLocation());
	if (!tmp) return false;

	auto it = std::find(tmp->mMeshes.begin(), tmp->mMeshes.end(), mesh);
	if (it != tmp->mMeshes.end())
	{
		tmp->mMeshes.erase(it);

	}

	collapse();
	return true;
}

OctTree* OctTree::insert(Mesh* mesh)
{
	OctTree* tmp = find(mesh->getLocation());
	if (!tmp) return nullptr;

	tmp->mMeshes.emplace_back(mesh);

	if (tmp->mMeshes.size() > MaxObject && mBbox.Extent.length() > MinExtentSize)
	{
		tmp->divide();

		//auto tmpmesh = tmp->mMeshes;
		

		//delete all meshes and push to new child node
		for(auto m: tmp->mMeshes)
			insert(m);

		tmp->mMeshes.clear();
	}
	collapse();
	return tmp;

}

OctTree* OctTree::find(const glm::vec3& location)
{

    if (isLeaf()) {
        return this;
    }
	for(auto child:mChilds)
	{
		if (child->mBbox.isOverlaping(location))
			return child->find(location);
	}
	return nullptr;
}

OctTree* OctTree::find(Mesh* mesh)
{
	if (isLeaf()) {
		for(Mesh* m: mMeshes)
		{
			if (m == mesh)
			{
				return this;
			}
		}
	}
	else for (OctTree* child : mChilds)
	{
			return child->find(mesh);
	}
	return nullptr;
}

void OctTree::move(Mesh* mesh)
{
	if(remove(mesh))
		insert(mesh);
}


void OctTree::drawTree()
{
	if(isLeaf() && !mMeshes.empty())
	{
		mBbox.draw();
	}
	/*else if (isLeaf())
	{
		mBbox.draw();
	}*/
	for (auto child : mChilds)
		child->drawTree();

}

bool OctTree::isLeaf() const
{
	return mChilds.empty();
}

int OctTree::sumGameObject()
{
	int sum = mMeshes.size();
	for(auto child: mChilds)
	{
		sum += child->sumGameObject();
	}
	return sum;
}
