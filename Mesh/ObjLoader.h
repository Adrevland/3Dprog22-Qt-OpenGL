#pragma once
#include <vector>

#include "Math/Vertex.h"

//F Helper struct


class ObjLoader
{
public:
    ObjLoader(const char* ObjFile);
    ~ObjLoader();

    void init();
    std::vector<Vertex> getVertices() { return mVertices; }
    std::vector<unsigned int> getIndices() { return mIndices; }

private:


    std::string filepath;
    std::string mMeshName;

    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
};

